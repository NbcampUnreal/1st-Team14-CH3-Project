#include "Weapon/CWeaponStructures.h"
#include "GameFramework/Character.h"

void FDoActionData::DoAction(ACharacter* Owners)
{
	if (Montage != nullptr)
		Owners->PlayAnimMontage(Montage, PlayRate);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void FHitData::SnedDamage(APawn* InAttacker, AActor* InAttackCauser, APawn* InOther)
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
