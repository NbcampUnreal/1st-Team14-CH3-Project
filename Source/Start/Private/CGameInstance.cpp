#include "CGameInstance.h"
#include "CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "CHUDWidget.h"
#include "GameFramework/HUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CStatusComponent.h"

UCGameInstance::UCGameInstance()
{
    PlayerHealth = 100.0f;  // 기본 체력 값
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

void UCGameInstance::SetPlayerHealth(float NewHealth)
{
    PlayerHealth = FMath::Clamp(NewHealth, 0.0f, 100.0f);
}

// 체력 초기화
void UCGameInstance::ResetPlayerHealth()
{
    PlayerHealth = 100.0f;
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
                UE_LOG(LogTemp, Warning, TEXT("✅ SavePlayerState: 저장된 체력 값: %f"), PlayerHealth);
            }
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
                StatusComponent->HealHealth(PlayerHealth - StatusComponent->GetHealth());
            }
        }
    }
    // ✅ 점수 불러오기
    int LoadedScore = GetScore();
    Score = LoadedScore;  // ✅ Score 값을 다시 할당
    UE_LOG(LogTemp, Warning, TEXT("✅ LoadPlayerState: 불러온 점수: %d"), LoadedScore);

    // ✅ HUD 업데이트
    NotifyHUDScoreUpdate();

    // ✅ 불러온 체력과 점수를 디버깅 로그로 출력
    UE_LOG(LogTemp, Warning, TEXT("✅ LoadPlayerState: 불러온 체력: %f, 불러온 점수: %d"), PlayerHealth, LoadedScore);
}

// ✅ HUD에 점수 업데이트 알림을 보냄
void UCGameInstance::NotifyHUDScoreUpdate()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        UCHUDWidget* HUDWidget = nullptr;

        // ✅ Viewport에서 UCHUDWidget을 직접 탐색
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
            UE_LOG(LogTemp, Warning, TEXT("❌ HUD 위젯을 찾을 수 없음, 0.1초 후 다시 시도"));
            FTimerHandle TimerHandle;
            PC->GetWorldTimerManager().SetTimer(TimerHandle, this, &UCGameInstance::NotifyHUDScoreUpdate, 0.1f, false);
        }
    }
}

// ✅ 게임오버 후 체력과 점수 초기화
void UCGameInstance::ResetPlayerState()
{
    ResetPlayerHealth();  // 체력 100으로 초기화
    ResetScore();         // 점수 0으로 초기화
}