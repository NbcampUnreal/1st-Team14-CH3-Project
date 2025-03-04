#include "CCharacter.h"
#include "CGameInstance.h"
#include "Components/CCameraComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Actor.h"

ACCharacter::ACCharacter()
{
    //  이동 컴포넌트 추가
    StateComponent = CreateDefaultSubobject<UCStateComponent>(TEXT("StateComponent"));
    MovementComponent = CreateDefaultSubobject<UCMovementComponent>(TEXT("MovementComponent"));
    WeaponComponent = CreateDefaultSubobject<UCWeaponComponent>(TEXT("WeaponComponent"));
    MontagesComponent = CreateDefaultSubobject<UCMontagesComponent>(TEXT("MontagesComponent"));
    StatusComponent = CreateDefaultSubobject<UCStatusComponent>(TEXT("StatusComponent"));
    FeetComponent = CreateDefaultSubobject<UCFeetComponent>(TEXT("FeetComponent"));
    // 🔹 기본 체력 값 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;
    bIsDead = false;
}

void ACCharacter::BeginPlay()
{
    Super::BeginPlay();

	LoadHealthFromGameInstance();

    if (StateComponent)
    {
        // 🔹 델리게이트를 현재 캐릭터의 `HandleStateChanged()`에 연결
        StateComponent->OnStateTypeChanged.AddDynamic(this, &ACCharacter::HandleStateChanged);
    }
}

void ACCharacter::HandleStateChanged(EStateType PreviousType, EStateType NewType)
{
    UE_LOG(LogTemp, Warning, TEXT("🔹 상태 변경: %d → %d"), (int32)PreviousType, (int32)NewType);

    switch (NewType)
    {
    case EStateType::Hitted:
        Hitted();
        break;

    case EStateType::Dead:
        Die();
        break;

    default:
        break;
    }
}

void ACCharacter::Hitted()
{
    if (StatusComponent)
    {
        StatusComponent->Damage(HittedInfo.Power);

        if (!StatusComponent->IsDead()) // 🔹 살아있으면 피격 애니메이션 실행
        {
            if (MontagesComponent)
            {
                MontagesComponent->PlayHitMode(); // 🔹 PlayHit() → PlayHitMode()로 변경
            }
            return;
        }
    }

    // 🔹 사망 상태로 전환
    StateComponent->SetDeadMode();
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

float ACCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

void ACCharacter::ModifyHealth(float Amount)
{
    if (bIsDead) return;

    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

    if (Health <= 0.0f)
    {
        StateComponent->SetDeadMode();
        Die();
    }

    UE_LOG(LogTemp, Warning, TEXT("체력 변경: %f"), Health);
}

float ACCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (StatusComponent->IsDead()) return 0.0f; // 사망한 경우 데미지 무효

    // 🔹 구조체에 데미지 정보 저장
    HittedInfo.Power = damage;
    HittedInfo.Character = Cast<ACharacter>(DamageCauser);
    HittedInfo.Causer = DamageCauser;

    // 🔹 상태 변경 (SetHittedMode() 사용)
    StateComponent->SetHittedMode();  // 내부적으로 ChangeType(Hitted) 실행됨

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
    if (MontagesComponent)
    {
        MontagesComponent->PlayDeadMode();
    }
}


//bool ACCharacter::IsPlayerControlled() const
//{
//    AController* Controller = GetController();
//    return (Controller && Controller->IsPlayerController());
//}
