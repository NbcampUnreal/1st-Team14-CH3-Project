#include "AElevatorTrigger.h"
#include "Components/BoxComponent.h"
#include "CGameState.h"
#include "Kismet/GameplayStatics.h"

AAElevatorTrigger::AAElevatorTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    // 🔹 트리거 박스 컴포넌트 생성
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = BoxComponent;

    // 🔹 오버랩 이벤트 연결
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAElevatorTrigger::OnOverlap);
}

void AAElevatorTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void AAElevatorTrigger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    ACPlayer* PlayerCharacter = Cast<ACPlayer>(OtherActor);
    if (!PlayerCharacter) return;

    if (ElevatorSound)
    {
        PlayerCharacter->DisableInput(Cast<APlayerController>(PlayerCharacter->GetController()));

        float SoundDuration = ElevatorSound->GetDuration();
        UGameplayStatics::PlaySound2D(this, ElevatorSound);

        // 연구소 미로(N, Labyinth)인지 보스 연구소(BossArea)인지에 따라 다음 레벨 설정
        EGameState NewState = bIsN_Elevator ? EGameState::Labyrinth : EGameState::BossArea;
        UE_LOG(LogTemp, Warning, TEXT("🔍 OnOverlap() -> 선택된 NewState 값: %d (0 = Lobby, 5 = Labyrinth, 6 = BossArea)"), static_cast<int32>(NewState));

        // 사운드 재생 후 레벨 전환 & 플레이어 이동
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, PlayerCharacter, NewState]()
            {
                LoadNextLevel(NewState);
            }, SoundDuration, false);
    }
}

void AAElevatorTrigger::LoadNextLevel(EGameState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("🔄 LoadNextLevel 호출됨 - 상태 변경: %d"), static_cast<int32>(NewState));

    // ✅ NewState 값이 올바른지 확인하는 추가 로그
    if (NewState == EGameState::Labyrinth)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔍 NewState = Labyrinth (미로맵 이동)"));
    }
    else if (NewState == EGameState::BossArea)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔍 NewState = BossArea (보스 연구소 이동)"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 잘못된 NewState 값이 전달됨: %d"), static_cast<int32>(NewState));
    }

    // ✅ 게임 상태 업데이트
    ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
    if (GameState)
    {
        GameState->SetGameState(NewState);
    }

    // ✅ GameInstance에서 올바른 RespawnTag 저장
    UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        FName SpawnTag;

        // NewState 값이 올바르게 들어오는지 확인 후 태그 설정
        if (NewState == EGameState::Labyrinth)
        {
            SpawnTag = FName("LabyrinthSpawn");
        }
        else if (NewState == EGameState::BossArea)
        {
            SpawnTag = FName("BossAreaSpawn");
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ NewState 값이 올바르지 않아 RespawnTag 설정 실패! 기본값 적용"));
            SpawnTag = FName("LabyrinthSpawn"); // 기본값 (혹시라도 잘못된 경우 대비)
        }

        GameInstance->SetRespawnTag(SpawnTag);
        UE_LOG(LogTemp, Warning, TEXT("✅ RespawnTag 설정 완료: %s"), *SpawnTag.ToString());
    }

    // ✅ 레벨 변경 수행
    UE_LOG(LogTemp, Warning, TEXT("🌍 맵 변경 진행 -> MAIN_MAP"));
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Map/LapMap/ModSci_Engineer/Maps/MAIN_MAP"));
}



void AAElevatorTrigger::MovePlayerToSpawn(ACPlayer* PlayerCharacter, EGameState State, int32 RetryCount)
{
    if (!PlayerCharacter || RetryCount <= 0) return;

    // ✅ `NewState` 값에 따라 정확한 스폰 태그 설정
    FName SpawnTag;
    if (State == EGameState::Labyrinth)
    {
        SpawnTag = FName("LabyrinthSpawn");
    }
    else if (State == EGameState::BossArea)
    {
        SpawnTag = FName("BossAreaSpawn");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 잘못된 상태로 인해 스폰 태그를 결정할 수 없음!"));
        return;
    }

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

        // 🔹 1초 후 다시 시도 (최대 5번)
        FTimerHandle RetryTimer;
        GetWorldTimerManager().SetTimer(RetryTimer, [this, PlayerCharacter, State, RetryCount]()
            {
                MovePlayerToSpawn(PlayerCharacter, State, RetryCount - 1);
            }, 1.0f, false);
    }
}
