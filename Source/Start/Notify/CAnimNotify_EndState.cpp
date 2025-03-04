// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_EndState.h"

#include "Interface/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "End State";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	IICharacter* Interface = Cast<IICharacter>(MeshComp->GetOwner());
	if(Interface == nullptr)
		return;

	switch (Type)
	{
	case EStateType::Dead :
		Interface->End_Dead();
		break;
	case  EStateType::Hitted:
		Interface->End_Hit();
		break;
	}
}
