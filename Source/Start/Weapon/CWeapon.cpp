// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon.h"
#include "CCharacter.h"
#include "Camera/CameraComponent.h"

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
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	
	return !b;
}

void ACWeapon::Equip()
{
	bEquipping = true;
	if (EquipMontage == nullptr)
		return;
	
	OwnerCharacter->PlayAnimMontage(EquipMontage,Equip_PlayRate);
}

void ACWeapon::BeginEquip()
{
	if (RightHandSokcetName.IsValid())
		AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), RightHandSokcetName);
}

void ACWeapon::EndEquip()
{
	bEquipping =false;
}

bool ACWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	
	return !b;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), HolsterSocketName);
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon::BeginFire()
{
	bFiring = true;

	OnFireing();
}

void ACWeapon::EndFire()
{
	bFiring = false;
}

void ACWeapon::OnFireing()
{
	UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (camera == nullptr)
		return;

	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	FVector end = transform.GetLocation() + direction*HitDistance;
	if (Debug == true)	
		DrawDebugLine(GetWorld(),start,end,DebugColor,true,LifeTime);
}

