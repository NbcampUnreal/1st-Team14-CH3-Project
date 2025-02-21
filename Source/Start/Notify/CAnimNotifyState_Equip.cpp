// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_Equip.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh is NULL"));
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh's Owner is NULL"));
		return;
	}
	UCStateComponent* state = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Weapon Component is NULL"));
		return;
	}
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Weapon Component is NULL"));
		return;
	}
	state->SetEquipMode();
	weapon->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh is NULL"));
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh's Owner is NULL"));
		return;
	}
	UCStateComponent* state = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Weapon Component is NULL"));
		return;
	}
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Weapon Component is NULL"));
		return;
	}
	state->SetIdleMode();
	weapon->End_Equip();
}
