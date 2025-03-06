#include "CGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CCharacter.h"
#include "Blueprint/UserWidget.h"
#include "CHUDWidget.h"
#include "GameFramework/HUD.h" 

ACGameState::ACGameState()
{
    CurrentState = EGameState::MenuMap; // 게임 시작 시 메뉴 상태
}

void ACGameState::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(EGameState::MenuMap); // 게임 시작 시 메뉴 상태로 초기화

    // 🔹 1초마다 CheckScoreForRedDoor()를 실행하는 타이머 설정
    GetWorldTimerManager().SetTimer(ScoreCheckTimer, this, &ACGameState::CheckScoreForRedDoor, 1.0f, true);
    // 🔹 1초마다 중간 보스 사망 여부를 체크하는 타이머 설정
    GetWorldTimerManager().SetTimer(MidBossCheckTimer, this, &ACGameState::CheckMidBossDefeated, 1.0f, true);

    FString CurrentMapName = GetWorld()->GetMapName();

    if (CurrentMapName.Contains(TEXT("MAIN_MAP")))
    {
        // ✅ GameInstance에서 리스폰 태그 가져오기
        UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if (GameInstance)
        {
            FName RespawnLocation = GameInstance->GetRespawnTag();
            if (RespawnLocation != NAME_None)
            {
                UE_LOG(LogTemp, Warning, TEXT("✅ 연구소 맵 - %s에서 리스폰"), *RespawnLocation.ToString());

                UCHUDWidget* HUDWidget = Cast<UCHUDWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
                if (HUDWidget)
                {
                    HUDWidget->RespawnPlayerAtTaggedSpawnPoint(RespawnLocation);
                    GameInstance->SetRespawnTag(NAME_None); // ✅ 리스폰 후 태그 초기화
                }
            }
        }
    }
}

void ACGameState::OnLevelLoaded()
{
    UE_LOG(LogTemp, Warning, TEXT("🔄 레벨 로드 완료! 플레이어를 스폰 태그에 맞게 이동"));

    // ✅ GameInstance에서 저장된 리스폰 태그 가져오기
    UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance) return;

    FName RespawnTag = GameInstance->GetRespawnTag();
    if (RespawnTag == NAME_None) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController) return;

    ACPlayer* PlayerCharacter = Cast<ACPlayer>(PlayerController->GetPawn());
    if (!PlayerCharacter) return;

    // ✅ 1초 후 MovePlayerToSpawn 실행 (레벨이 완전히 로드된 후 안전한 시점)
    FTimerHandle RespawnTimer;
    GetWorldTimerManager().SetTimer(RespawnTimer, [this, PlayerCharacter, RespawnTag]()
        {
            MovePlayerToSpawn(PlayerCharacter, RespawnTag, 5);

        }, 1.0f, false); // 🔹 1초 딜레이 추가 (레벨이 완전히 로드된 후 실행)
}

void ACGameState::MovePlayerToSpawn(AActor* PlayerCharacter, FName SpawnTag, int32 RetryCount)
{
    if (!PlayerCharacter || RetryCount <= 0) return;

    UE_LOG(LogTemp, Warning, TEXT("🔍 MovePlayerToSpawn() -> 사용된 SpawnTag: %s"), *SpawnTag.ToString());

    TArray<AActor*> FoundSpawnPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnTag, FoundSpawnPoints);

    if (FoundSpawnPoints.Num() > 0)
    {
        AActor* SpawnPoint = FoundSpawnPoints[0];
        PlayerCharacter->SetActorLocation(SpawnPoint->GetActorLocation());
        PlayerCharacter->SetActorRotation(SpawnPoint->GetActorRotation());

        UE_LOG(LogTemp, Warning, TEXT("✅ [%s]에서 플레이어 스폰 완료!"), *SpawnTag.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⏳ [%s] 태그를 찾지 못함. 다시 시도 중... (남은 횟수: %d)"), *SpawnTag.ToString(), RetryCount - 1);
        FTimerHandle RetryTimer;
        GetWorldTimerManager().SetTimer(RetryTimer, [this, PlayerCharacter, SpawnTag, RetryCount]()
            {
                MovePlayerToSpawn(PlayerCharacter, SpawnTag, RetryCount - 1);
            }, 1.0f, false);
    }
}


// ✅ 게임 오버 UI 표시 (플레이어 사망 시 호출)
void ACGameState::ShowGameOverUI()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    if (HUDWidgetClass)
    {
        UCHUDWidget* HUDWidget = CreateWidget<UCHUDWidget>(PC, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            HUDWidget->ShowGameOverUI();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());
            PC->SetInputMode(InputMode);  // 먼저 입력 모드를 UI로 설정
            PC->SetShowMouseCursor(true); // 이후 마우스 커서 활성화

            // ✅ UI 포커스를 강제 설정 (버튼 클릭 가능하게)
            HUDWidget->SetUserFocus(PC);  // 먼저 유저 포커스를 설정
            HUDWidget->SetKeyboardFocus();  // 이후 키보드 포커스를 설정

            UE_LOG(LogTemp, Warning, TEXT("게임 오버 UI 표시됨, 마우스 클릭 가능"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HUDWidgetClass가 설정되지 않았습니다!"));
    }
}


void ACGameState::CheckScoreForRedDoor()
{
    UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
    if (!GameInstance) return;

    if (GameInstance->GetScore() >= 100)
    {
        TArray<AActor*> FoundDoors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("RedDoor"), FoundDoors);

        for (AActor* DoorActor : FoundDoors)
        {
            if (DoorActor)
            {
                // 🔹 블루프린트에서 만든 'Event OpenDoor' 실행
                UFunction* OpenDoorFunction = DoorActor->FindFunction(FName("OpenDoor"));
                if (OpenDoorFunction)
                {
                    DoorActor->ProcessEvent(OpenDoorFunction, nullptr);
                    UE_LOG(LogTemp, Warning, TEXT("레드도어가 부드럽게 열리도록 블루프린트 이벤트 실행됨!"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("OpenDoor 함수가 없습니다! 블루프린트 확인 필요!"));
                }
            }
        }
    }
}

void ACGameState::CheckMidBossDefeated()
{
    TArray<AActor*> FoundBosses;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MidBoss"), FoundBosses);

    // 🔹 "MidBoss" 태그가 있는 액터가 남아있는지 확인
    if (FoundBosses.Num() == 0) // 중간 보스가 전부 죽었다면
    {
        // 🔹 엘리베이터 문 열기
        TArray<AActor*> FoundDoors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ElevatorDoor"), FoundDoors);

        for (AActor* DoorActor : FoundDoors)
        {
            if (DoorActor)
            {
                // 🔹 블루프린트에서 만든 'OpenDoor' 실행
                UFunction* OpenDoorFunction = DoorActor->FindFunction(FName("OpenDoor"));
                if (OpenDoorFunction)
                {
                    DoorActor->ProcessEvent(OpenDoorFunction, nullptr);
                    UE_LOG(LogTemp, Warning, TEXT("중간 보스가 사망하여 엘리베이터 문이 부드럽게 열립니다!"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("OpenDoor 함수가 없습니다! 블루프린트 확인 필요!"));
                }
            }
        }
    }
}

void ACGameState::SetGameState(EGameState NewState)
{
    if (CurrentState == NewState)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 현재 상태입니다: %d"), static_cast<int32>(NewState));
        return; // 🔹 동일한 상태로 변경 시 다시 실행하지 않도록 방지
    }

    CurrentState = NewState;

    FString CurrentMapName = GetWorld()->GetMapName();

    switch (CurrentState)
    {
    case EGameState::MenuMap:
        UE_LOG(LogTemp, Warning, TEXT("게임 상태: 메뉴"));
        // 🔹 현재 맵이 이미 MenuLevel이면 다시 로드하지 않음
        if (CurrentMapName.Contains(TEXT("MenuLevel")))
        {
            return;
        }

        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/MenuLevel"));
        break;

    case EGameState::CityMap:
        UE_LOG(LogTemp, Warning, TEXT("도시 맵 로드"));
        // 🔹 현재 맵이 이미 CityMap이면 다시 로드하지 않음
        if (CurrentMapName.Contains(TEXT("Map_Post-Apocalyptic_NightLight")))
        {
            return;
        }

        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/PA_UrbanCity/Maps/Map_Post-Apocalyptic_NightLight"));
        break;

    case EGameState::MercenaryBattle:
        UE_LOG(LogTemp, Warning, TEXT("용병단과 전투 시작"));
        break;

    case EGameState::ScoreCheck:
        UE_LOG(LogTemp, Warning, TEXT("점수 체크 진행"));
        break;

    case EGameState::ElevatorSelection:
        UE_LOG(LogTemp, Warning, TEXT("엘리베이터 선택"));
        break;

    case EGameState::Labyrinth:
        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/LapMap/ModSci_Engineer/Maps/MAIN_MAP"));
        break;

    case EGameState::BossArea:
        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/LapMap/ModSci_Engineer/Maps/MAIN_MAP"));
        break;

    case EGameState::BossFight:
        UE_LOG(LogTemp, Warning, TEXT("보스전 시작"));
        break;

    case EGameState::Symbiosis:
        UE_LOG(LogTemp, Warning, TEXT("심비오 공생"));
        break;

    case EGameState::Ending1:
        UE_LOG(LogTemp, Warning, TEXT("엔딩1: 새로운 인류"));
        break;

    case EGameState::Ending2:
        UE_LOG(LogTemp, Warning, TEXT("엔딩2: 자유로 돌아가다"));
        break;

    case EGameState::GameOver:
        UE_LOG(LogTemp, Warning, TEXT("게임 오버"));
        break;

    default:
        break;
    }
}

//void ACGameState::SetGameState(EGameState NewState)
//{
//    if (NewState == EGameState::Playing)
//    {
//        // 🔹 게임 인스턴스에 현재 체력 저장
//        ACCharacter* Player = Cast<ACCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//        if (Player)
//        {
//            Player->SaveHealthToGameInstance();
//        }
//
//        // 🔹 다음 맵 로드
//        UGameplayStatics::OpenLevel(GetWorld(), "NextLevel");
//    }
//
//    CurrentState = NewState;
//}