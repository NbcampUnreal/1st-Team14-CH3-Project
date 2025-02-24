// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_Reload.h"

#include "Components/CWeaponComponent.h"

FString UCAnimNotify_Reload::GetNotifyName_Implementation() const
{
	return "Reload";
}

void UCAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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

	
}
