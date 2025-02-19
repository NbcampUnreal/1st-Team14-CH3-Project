#include "CGameInstance.h"

UCGameInstance::UCGameInstance()
{
    PlayerHealth = 100.0f;  // �⺻ ü�� ��
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