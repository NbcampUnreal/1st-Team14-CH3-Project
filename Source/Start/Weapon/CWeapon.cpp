// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon.h"
#include "CCharacter.h"

// Sets default values
ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
		return;
	
	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), HolsterSocketName);
}

// Called every frame
void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACWeapon::CanEquip()
{
	return true;
}

void ACWeapon::Equip()
{
	Mesh->SetVisibility(true);
}

void ACWeapon::BeginEquip()
{
}

void ACWeapon::EndEquip()
{
}

bool ACWeapon::CanUnequip()
{
	return true;
}

void ACWeapon::Unequip()
{
	Mesh->SetVisibility(false);
}

