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
#include "Weapon/CWeaponStructures.h"
#include "CPlayer.h"
#include "CGameState.h"

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

	//LoadHealthFromGameInstance();

    if (StateComponent)
    {
        // 🔹 델리게이트를 현재 캐릭터의 `HandleStateChanged()`에 연결
        StateComponent->OnStateTypeChanged.AddDynamic(this, &ACCharacter::HandleStateChanged);
    }
    UAnimInstance* instance = GetMesh()->GetAnimInstance();
    if (instance != nullptr)
    	instance->OnMontageEnded.AddDynamic(this, &ACCharacter::HandleAnyMontageEnded);
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
    //auto a = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
    UAnimInstance* instance = GetMesh()->GetAnimInstance();
    if (instance != nullptr)
    {
    	instance->Montage_Stop(0.1f);
        UAnimMontage* montage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
        if (montage != nullptr)
            HandleAnyMontageEnded(montage, true);
    }
    StatusComponent->Damage(HittedInfo.Power);
    if(StatusComponent->GetHealth() <= 0.0f)
	    StateComponent->SetDeadMode();
    else if(StatusComponent->GetHealth() > 0.0f && bIsHit == false)
        HittedInfo.Event->HitData->PlayMontage(this);
    if (bIsHit == true)
         End_Hit();
}


//void ACCharacter::SaveHealthToGameInstance()
//{
//    UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
//    if (GameInstance)
//    {
//        GameInstance->SetPlayerHealth(Health);
//        UE_LOG(LogTemp, Warning, TEXT("체력 저장: %f"), Health);
//    }
//}
//
//void ACCharacter::LoadHealthFromGameInstance()
//{
//    UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
//    if (GameInstance)
//    {
//        Health = GameInstance->GetPlayerHealth();
//        UE_LOG(LogTemp, Warning, TEXT("체력 로드: %f"), Health);
//    }
//}

void ACCharacter::HandleAnyMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if(Montage == nullptr)
        return;
    if(Montage->GetName() == "Equip_Rifle_Standing_Montage")
    {
        WeaponComponent->Begin_Equip();
        WeaponComponent->End_Equip();
        return;
    }
    if(Montage->GetName() == "Reload_Rifle_Hip_Montage")
    {
	    WeaponComponent->Load_Magazine();
	    WeaponComponent->End_Magazine();
	    return;
    }
}

void ACCharacter::End_Hit()
{
	IICharacter::End_Hit();
    StateComponent->SetIdleMode();
	bIsHit = false;
}

void ACCharacter::End_Dead()
{
	bIsHit = false;
    Destroy();
}

//float ACCharacter::GetHealth() const
//{
//    return Health;
//}
//
//float ACCharacter::GetMaxHealth() const
//{
//    return MaxHealth;
//}

//void ACCharacter::ModifyHealth(float Amount)
//{
//    if (bIsDead) return;
//
//    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
//
//    if (Health <= 0.0f)
//    {
//        StateComponent->SetDeadMode();
//        Die();
//    }
//
//    UE_LOG(LogTemp, Warning, TEXT("체력 변경: %f"), Health);
//}

float ACCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (StateComponent->IsDeadMode() == true)
        return 0;


	HittedInfo.Event = (FActionDamageEvent*)&DamageEvent;
    HittedInfo.Power = damage;
    HittedInfo.Character = Cast<ACharacter>(EventInstigator->GetPawn());
    HittedInfo.Causer = DamageCauser;
    
    if (StateComponent->GetStateType() != EStateType::Idle)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(L"%d", StateComponent->GetStateType()));
        bIsHit = true;
        Hitted();
		return DamageAmount;
    }
    // 🔹 상태 변경 (SetHittedMode() 사용)
    StateComponent->SetHittedMode();  // 내부적으로 ChangeType(Hitted) 실행됨

    return DamageAmount;
}

void ACCharacter::Heal(float HealAmount)
{
    if (bIsDead) return;

	StatusComponent->HealHealth(HealAmount);
    //ModifyHealth(HealAmount);

    UE_LOG(LogTemp, Warning, TEXT("캐릭터가 %f 체력을 회복함"), HealAmount);
}

void ACCharacter::Die()
{
    if (MontagesComponent)
    {
        MontagesComponent->PlayDeadMode();
    }

    // 🔹 적(Enemy)인지 플레이어인지 구분
    ACPlayer* PlayerCharacter = Cast<ACPlayer>(this); // 플레이어인지 확인
    if (PlayerCharacter) // 플레이어가 죽었을 때만 실행
    {
        ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
        if (GameState)
        {
            GameState->ShowGameOverUI(); // 🎯 게임 오버 UI 표시
        }
    }
}


//bool ACCharacter::IsPlayerControlled() const
//{
//    AController* Controller = GetController();
//    return (Controller && Controller->IsPlayerController());
//}
