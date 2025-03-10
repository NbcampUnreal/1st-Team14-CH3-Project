// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Throw.h"

#include "CCharacter.h"
#include "CGrenadesItem.h"
#include "CWeaponStructures.h"
#include "Camera/CameraComponent.h"
#include "Components/CCameraComponent.h"
#include "Components/CStateComponent.h"
#include "Kismet/GameplayStatics.h"

ACWeapon_Throw::ACWeapon_Throw()
{
}

void ACWeapon_Throw::BeginPlay()
{
	Super::BeginPlay();
}

void ACWeapon_Throw::BeginEquip()
{
	Create();
}

void ACWeapon_Throw::Unequip()
{
	Super::Unequip();
	for (int32 i = Grenades.Num() - 1; i >= 0; i--)
	{
		if (Grenades[i]->GetAttachParentActor())
			Grenades[i]->Destroy();
	}
}

void ACWeapon_Throw::DonAction()
{
	if (State->IsIdleMode() == false)
		return;
	Super::DonAction();

	Data.DoAction(OwnerCharacter);
}

void ACWeapon_Throw::BeginAction()
{
	Super::BeginAction();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Shoot")));
	ACGrenadesItem* greade = GetAttached();
	if (greade)
	{
		greade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
		greade->Shoot(OwnerCharacter, camera->GetForwardVector());
		if(greade != nullptr)
			Grenades.Remove(greade);
	}
}

void ACWeapon_Throw::EndAction()
{
	Super::EndAction();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Create")));
	Create();
}

void ACWeapon_Throw::Create()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Grenade_Hand");
	FRotator rotation = OwnerCharacter->GetMesh()->GetSocketRotation("Greade_Hand");
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	params.Owner = OwnerCharacter;
	ACGrenadesItem* grenade = GetWorld()->SpawnActor<ACGrenadesItem>(GrenadesClass, location, rotation, params);
	if (grenade)
	{
		grenade->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Grenade_Hand");
		Grenades.Add(grenade);
	}

}

ACGrenadesItem* ACWeapon_Throw::GetAttached()
{
	for (ACGrenadesItem* greade : Grenades)
		if (greade != nullptr)
			return greade;

	return nullptr;
}
