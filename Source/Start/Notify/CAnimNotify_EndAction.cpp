// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_EndAction.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End Action";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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

	weapon->End_DoAction();
}
