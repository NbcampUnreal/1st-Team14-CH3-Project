// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Knife.h"

#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/SphereComponent.h"

ACWeapon_Knife::ACWeapon_Knife()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/MilitaryWeapSilver/Weapons/Knife_A.Knife_A'"));
	if (mesh.Succeeded() == true)
		Mesh->SetSkeletalMesh(mesh.Object);
	Knife = CreateDefaultSubobject<UCapsuleComponent>("Knife");
	Knife->SetupAttachment(Mesh);
	Fist = CreateDefaultSubobject<USphereComponent>("Fist");
	Fist->SetupAttachment(Scene);
	RightHandSokcetName = "Knife";
	FistHandSokcetName = "Fist";
}

void ACWeapon_Knife::BeginPlay()
{
	Super::BeginPlay();
	if(RightHandSokcetName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSokcetName);
	if (Fist != nullptr && FistHandSokcetName.IsValid() == true)
		Fist->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FistHandSokcetName);
	Mesh->SetVisibility(false);
}

void ACWeapon_Knife::BeginEquip()
{
	Super::BeginEquip();
	Mesh->SetVisibility(true);
}

void ACWeapon_Knife::Unequip()
{
	Super::Unequip();
	Mesh->SetVisibility(false);
}

void ACWeapon_Knife::DonAction()
{
	if (Data.Num() < 1)
		return;
	if(bEnable == true)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	if(State->IsIdleMode() == false)
		return;
	Super::DonAction();

	Data[Index].DoAction(OwnerCharacter);
}

void ACWeapon_Knife::BeginAction()
{
	Super::BeginAction();
	if(bExist == false)
		return;

	bExist = false;
	Data[++Index].DoAction(OwnerCharacter);
}

void ACWeapon_Knife::EndAction()
{
	Super::EndAction();

	Index = 0;
}
