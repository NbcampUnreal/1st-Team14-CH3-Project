// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_Collision.h"

#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"
#include "Weapon/CWeapon_Knife.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}


void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is NULL"));
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh's Owner is NULL"));
		return;
	}
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Component is NULL"));
		return;
	}

	ACWeapon_Knife* curr = Cast<ACWeapon_Knife>(weapon->GetCurrentWeapon());
	if (curr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is NULL"));
		return;
	}
	curr->EnableCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is NULL"));
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh's Owner is NULL"));
		return;
	}
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Component is NULL"));
		return;
	}

	ACWeapon_Knife* curr = Cast<ACWeapon_Knife>(weapon->GetCurrentWeapon());
	if (curr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is NULL"));
		return;
	}
	curr->DisableCollision();
}
