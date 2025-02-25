// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotifyState_ApplyDamage.h"
#include "Components/SphereComponent.h"

void UCAnimNotifyState_ApplyDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	USphereComponent* CollisionSphere = Cast<USphereComponent>(Owner->FindComponentByClass<USphereComponent>());
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UCAnimNotifyState_ApplyDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	USphereComponent* CollisionSphere = Cast<USphereComponent>(Owner->FindComponentByClass<USphereComponent>());
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
