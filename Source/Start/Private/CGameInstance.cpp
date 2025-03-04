#include "CGameInstance.h"

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

int UCGameInstance::GetScore() const
{
    return Score;
}

void UCGameInstance::AddScore(int ScoreAmount)
{
    Score += ScoreAmount;

    GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Score : %d"), Score));
}
