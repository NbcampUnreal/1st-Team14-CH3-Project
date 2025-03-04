#include "Weapon/CWeaponStructures.h"
#include "GameFramework/Character.h"

void FHitData::SnedDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;
	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	if(HitMontage == nullptr)
		return;
	InOwner->PlayAnimMontage(HitMontage,PlayRate);
}

void FHitData::PlaySound(ACharacter* InOwner)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

ACWeaponStructures::ACWeaponStructures()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACWeaponStructures::BeginPlay()
{
	Super::BeginPlay();
}
