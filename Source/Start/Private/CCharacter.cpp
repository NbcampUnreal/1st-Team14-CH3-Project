#include "CCharacter.h"
#include "CGameInstance.h"
#include "Components/CCameraComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Actor.h"

ACCharacter::ACCharacter()
{
    //  이동 컴포넌트 추가
    MovementComponent = CreateDefaultSubobject<UCMovementComponent>(TEXT("MovementComponent"));
    WeaponComponent = CreateDefaultSubobject<UCWeaponComponent>(TEXT("WeaponComponent"));
    MontagesComponent = CreateDefaultSubobject<UCMontagesComponent>("Montages Component");
    // 🔹 기본 체력 값 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;
    bIsDead = false;
}

void ACCharacter::BeginPlay()
{
    Super::BeginPlay();

    LoadHealthFromGameInstance();
}
void ACCharacter::SaveHealthToGameInstance()
{
    UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->SetPlayerHealth(Health);
        UE_LOG(LogTemp, Warning, TEXT("체력 저장: %f"), Health);
    }
}

void ACCharacter::LoadHealthFromGameInstance()
{
    UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        Health = GameInstance->GetPlayerHealth();
        UE_LOG(LogTemp, Warning, TEXT("체력 로드: %f"), Health);
    }
}

float ACCharacter::GetHealth() const
{
    return Health;
}

void ACCharacter::ModifyHealth(float Amount)
{
    if (bIsDead) return;

    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

    if (Health <= 0.0f)
    {
        Die();
    }

    UE_LOG(LogTemp, Warning, TEXT("체력 변경: %f"), Health);
}

float ACCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0.0f; // 사망 상태라면 데미지 무효

    ModifyHealth(-DamageAmount);

    UE_LOG(LogTemp, Warning, TEXT("캐릭터가 %f 데미지를 받음"), DamageAmount);

    return DamageAmount;
}

void ACCharacter::Heal(float HealAmount)
{
    if (bIsDead) return;

    ModifyHealth(HealAmount);

    UE_LOG(LogTemp, Warning, TEXT("캐릭터가 %f 체력을 회복함"), HealAmount);
}

void ACCharacter::Die()
{
    if (bIsDead) return;

    bIsDead = true;
    UE_LOG(LogTemp, Warning, TEXT("캐릭터 사망!"));

    //// 🔹 플레이어인지 아닌지 판단하여 사망 처리 방식 결정
    //if (IsPlayerControlled())
    //{
    //    // 🔹 플레이어가 사망하면 게임 오버 처리
    //    ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
    //    if (GameState)
    //    {
    //        GameState->SetGameState(EGameState::GameOver);
    //    }
    //}
    //else
    //{
    //    // 🔹 AI 적은 사망 시 그냥 소멸
    //    Destroy();
    //}
}

//bool ACCharacter::IsPlayerControlled() const
//{
//    AController* Controller = GetController();
//    return (Controller && Controller->IsPlayerController());
//}
