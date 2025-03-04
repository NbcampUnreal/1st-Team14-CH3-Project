// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Throw.h"

#include "CCharacter.h"
#include "CGrenadesItem.h"

ACWeapon_Throw::ACWeapon_Throw()
{
}

void ACWeapon_Throw::BeginPlay()
{
	Super::BeginPlay();
}

void ACWeapon_Throw::BeginEquip()
{
	FVector locaton = OwnerCharacter->GetMesh()->GetSocketLocation("Grenade");
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACGrenadesItem* Grenades = GetWorld()->SpawnActor<ACGrenadesItem>(GrenadesClass, locaton,locaton.Rotation(),param);
	FName name = "Grenade";
	Grenades->GetSkeletalMesh()->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Grenade");
	
}

void ACWeapon_Throw::Unequip()
{
	Super::Unequip();
}

void ACWeapon_Throw::DonAction()
{
	Super::DonAction();
}

void ACWeapon_Throw::BeginAction()
{
	Super::BeginAction();
}

void ACWeapon_Throw::EndAction()
{
	Super::EndAction();
}
