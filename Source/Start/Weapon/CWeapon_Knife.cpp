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

void ACWeapon_Knife::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if(OtherActor == nullptr)
		return;
	ACharacter* hit = Cast<ACharacter>(OtherActor);
	if(hit == OwnerCharacter)
		return;
	if(hit == nullptr)
		return;
	
	for (ACharacter* character : Hits)
	{
		if(character == hit)
			return;
	}

	Hits.AddUnique(hit);
	

	for (ACharacter* character : Hits)
	{
		UE_LOG(LogTemp,Error,L"%s", *character->GetName())
		UGameplayStatics::ApplyDamage(character, 10, OwnerCharacter->GetController(), this, UDamageType::StaticClass());
	}*/
	if(OtherActor == nullptr)
		return;
	ACharacter* other = Cast<ACharacter>(OtherActor);
	if(other == nullptr || other == OwnerCharacter)
		return;
	for (ACharacter* hitted : Hits)
		if(hitted == other)
			return;

	Hits.AddUnique(other);
	if(Hits.Num() - 1 < Index)
		return;
	UE_LOG(LogTemp, Error, TEXT("%s"), *other->GetName());
	HitDatas[Index].SnedDamage(OwnerCharacter, this, other);
	//UGameplayStatics::ApplyDamage(other, Damage, OwnerCharacter->GetController(), this, UDamageType::StaticClass());

}

void ACWeapon_Knife::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hits.Empty();
}

void ACWeapon_Knife::EnableCollision()
{
	if (Index == 0)
		Collision1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		Collision2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Collisions[Index]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACWeapon_Knife::DisableCollision()
{
	if (Index == 0)
		Collision1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		Collision2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
