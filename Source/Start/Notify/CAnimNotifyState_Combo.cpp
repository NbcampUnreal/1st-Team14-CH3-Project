// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_Combo.h"

#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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

	ACWeapon* curr = weapon->GetCurrentWeapon();
	if(curr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is NULL"));
		return;
	}
	curr->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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

	ACWeapon* curr = weapon->GetCurrentWeapon();
	if (curr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is NULL"));
		return;
	}
	curr->DisableCombo();
}
