// Fill out your copyright notice in the Description page of Project Settings.

#include "CEnemySoldier.h"
#include "Weapon/CWeapon.h"
#include "Components/CWeaponComponent.h"

ACEnemySoldier::ACEnemySoldier()
{
	bIsGunUsed = true;
	bShouldOneFire = true;
}

void ACEnemySoldier::BeginPlay()
{
	Super::BeginPlay();
}

void ACEnemySoldier::EnemyAttackStart(bool bIsCloseRangeAttack)
{
	if (bIsEqueped)
	{
		GunAttackStart();
	}
}

void ACEnemySoldier::EnemyAttackEnd()
{
	if (bIsEqueped)
	{
		GunAttackEnd();
	}
}

void ACEnemySoldier::Equip()
{
	if (!bIsEqueped)
	{
		WeaponComponent->SetRifleMode();
		bIsEqueped = true;
		if (bShouldOneFire)
		{
			ToggleAutoFire();
		}
	}
}

void ACEnemySoldier::UnEquip()
{
	if (bIsEqueped && WeaponComponent->GetCurrentWeapon())
	{
		if (bShouldOneFire)
		{
			ToggleAutoFire();
		}
		WeaponComponent->SetUnarmedMode();
		bIsEqueped = false;
	}
}

void ACEnemySoldier::ToggleAutoFire()
{
	if (WeaponComponent->GetCurrentWeapon())
	{
		WeaponComponent->ToggleAutoFire();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Toggle")));
	}

}

void ACEnemySoldier::GunAttackStart()
{
	WeaponComponent->Begin_Fire();
}

void ACEnemySoldier::GunAttackEnd()
{
	WeaponComponent->End_Fire();
}