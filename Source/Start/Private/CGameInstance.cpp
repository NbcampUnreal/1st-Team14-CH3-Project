#include "CGameInstance.h"
#include "CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "CHUDWidget.h"
#include "GameFramework/HUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CStatusComponent.h"
#include "CGameState.h"

UCGameInstance::UCGameInstance()
{
    Score = 0;
}

void UCGameInstance::Init()
{
    Super::Init();
}

float UCGameInstance::GetPlayerHealth() const
{
    return PlayerHealth;
}

// 체력 초기화
void UCGameInstance::ResetPlayerHealth()
{
    PlayerHealth = PlayerMaxHealth;
}

int UCGameInstance::GetScore() const
{
    return Score;
}

// ✅ 점수 설정
void UCGameInstance::SetScore(int NewScore)
{
    Score = NewScore;
}

void UCGameInstance::AddScore(int ScoreAmount)
{
    Score += ScoreAmount;
}

// 점수 초기화
void UCGameInstance::ResetScore()
{
    Score = 0;
}

// ✅ 게임 종료 전 또는 맵 변경 전 플레이어 상태 저장
void UCGameInstance::SavePlayerState()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn());
        if (Player)
        {
            UCStatusComponent* StatusComponent = Player->GetStatusComponent();
            if (StatusComponent)
            {
                PlayerHealth = StatusComponent->GetHealth(); // ✅ 현재 체력 저장
                PlayerMaxHealth = StatusComponent->GetMaxHealth(); // ✅ 최대 체력 저장
                UE_LOG(LogTemp, Warning, TEXT("✅ SavePlayerState: 저장된 체력 값: %f"), PlayerHealth);
            }

            // ✅ 현재 위치에서 태그가 있는 스폰 지점을 찾음
            TArray<AActor*> FoundSpawnPoints;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LabyrinthSpawn"), FoundSpawnPoints);
            if (FoundSpawnPoints.Contains(Player))
            {
                RespawnTag = FName("LabyrinthSpawn");
            }
            else
            {
                RespawnTag = FName("BossAreaSpawn");
            }

            UE_LOG(LogTemp, Warning, TEXT("✅ SavePlayerState: 저장된 RespawnTag: %s"), *RespawnTag.ToString());
        }
    }

    SetScore(Score); // ✅ 점수 저장
    UE_LOG(LogTemp, Warning, TEXT("✅ SavePlayerState: 저장된 점수: %d"), Score);
}


// ✅ 게임 시작 시 또는 맵 변경 후 플레이어 상태 불러오기
void UCGameInstance::LoadPlayerState()
{
    UE_LOG(LogTemp, Warning, TEXT("LoadPlayerState() 실행됨"));

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn());
        if (Player)
        {
            UCStatusComponent* StatusComponent = Player->GetStatusComponent();
            if (StatusComponent)
            {
                // ✅ 기존 최대 체력 저장
                float OldMaxHealth = PlayerMaxHealth;

                // ✅ 새로운 최대 체력 불러오기
                PlayerMaxHealth = StatusComponent->GetMaxHealth();

                // ✅ 기존 체력이 최대 체력과 같으면 보정
                if (PlayerHealth == OldMaxHealth)
                {
                    PlayerHealth = PlayerMaxHealth;
                }

                UE_LOG(LogTemp, Warning, TEXT("✅ LoadPlayerState: 최대 체력 적용됨 -> %f"), PlayerMaxHealth);
            }
        }
    }
    
    // ✅ 클래스 멤버 `RespawnTag`를 사용하도록 수정
    UE_LOG(LogTemp, Warning, TEXT("🔍 LoadPlayerState() -> 불러온 RespawnTag: %s"), *this->RespawnTag.ToString());

    ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
    if (GameState)
    {
        // ✅ ACGameState::MovePlayerToSpawn()을 호출할 때, 올바른 인자 전달
        GameState->MovePlayerToSpawn(PC->GetPawn(), this->RespawnTag, 5);
    }

    // ✅ 점수 불러오기
    int LoadedScore = GetScore();
    Score = LoadedScore;
    UE_LOG(LogTemp, Warning, TEXT("✅ LoadPlayerState: 불러온 점수: %d"), LoadedScore);

    // ✅ HUD 업데이트
    NotifyHUDScoreUpdate();

    UE_LOG(LogTemp, Warning, TEXT("✅ LoadPlayerState: 불러온 체력: %f, 불러온 점수: %d"), PlayerHealth, LoadedScore);
}


// ✅ HUD에 점수 업데이트 알림을 보냄
void UCGameInstance::NotifyHUDScoreUpdate()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return; // ✅ NULL 체크 추가

    UCHUDWidget* HUDWidget = nullptr;
    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UCHUDWidget::StaticClass(), false);

    if (FoundWidgets.Num() > 0)
    {
        HUDWidget = Cast<UCHUDWidget>(FoundWidgets[0]);
    }

    if (HUDWidget)
    {
        HUDWidget->UpdateScore(Score);
        UE_LOG(LogTemp, Warning, TEXT("✅ NotifyHUDScoreUpdate() 호출됨 - 점수: %d"), Score);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ HUD 위젯을 찾을 수 없음, 0.2초 후 다시 시도"));
        FTimerHandle TimerHandle;
        PC->GetWorldTimerManager().SetTimer(TimerHandle, this, &UCGameInstance::NotifyHUDScoreUpdate, 0.2f, false);
    }
}

// ✅ 게임오버 후 체력과 점수 초기화
void UCGameInstance::ResetPlayerState()
{
    ResetPlayerHealth();  // 체력 100으로 초기화
    ResetScore();         // 점수 0으로 초기화
}