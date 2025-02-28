// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_BeginAction.h"

#include "Components/CWeaponComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin Action";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return;

	weapon->Begin_DoAction();
}
