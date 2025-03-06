// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Knife.h"

#include "CWeaponStructures.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ACWeapon_Knife::ACWeapon_Knife()
{
	ItemType = EItemType::EIT_Knife;
	GunType = EGunType::Knife;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/MilitaryWeapSilver/Weapons/Knife_A.Knife_A'"));
	if (mesh.Succeeded() == true)
		Mesh->SetSkeletalMesh(mesh.Object);
	Collision2 = CreateDefaultSubobject < UCapsuleComponent>("Punch");
	Collision2->SetupAttachment(RootComponent);
	Collision1 = CreateDefaultSubobject<UCapsuleComponent>("Knife");
	Collision1->SetupAttachment(Mesh);
	RightHandSokcetName = "Knife";
	FistHandSokcetName = "Fist";
}

void ACWeapon_Knife::BeginPlay()
{
	Super::BeginPlay();
	
	
	Collisions.Add(Collision1);
	Collisions.Add(Collision2);
	for (UCapsuleComponent* child: Collisions)
	{
		child->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon_Knife::OnComponentBeginOverlap);
		child->OnComponentEndOverlap.AddDynamic(this, &ACWeapon_Knife::OnComponentEndOverlap);
		child->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Mesh->SetVisibility(false);

	if (RightHandSokcetName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSokcetName);
	if (Collision2 != nullptr && FistHandSokcetName.IsValid() == true)
		Collision2->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FistHandSokcetName);
	
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
	if(AttackSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(OwnerCharacter->GetWorld(), AttackSound, FVector::ZeroVector, FRotator::ZeroRotator);
	Data[Index].DoAction(OwnerCharacter);
}

void ACWeapon_Knife::BeginAction()
{
	Super::BeginAction();
	if(bExist == false)
		return;

	bExist = false;
	if (AttackSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(OwnerCharacter->GetWorld(), AttackSound, FVector::ZeroVector, FRotator::ZeroRotator);
	Data[++Index].DoAction(OwnerCharacter);
}

void ACWeapon_Knife::EndAction()
{
	Super::EndAction();

	Index = 0;
}

void ACWeapon_Knife::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr)
		return;
	APawn* other = Cast<APawn>(OtherActor);
	if(other == nullptr || other == OwnerCharacter)
		return;
	for (APawn* hitted : Hits)
		if(hitted == other)
			return;

	Hits.AddUnique(other);
	if(Hits.Num() <= 0)
		return;
	HitDatas[Index].SnedDamage(OwnerCharacter, this, other);
}

void ACWeapon_Knife::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hits.Empty();
}

void ACWeapon_Knife::EnableCollision()
{
	Collisions[Index]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACWeapon_Knife::DisableCollision()
{
	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
