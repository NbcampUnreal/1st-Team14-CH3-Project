// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotifyState_ApplyDamage.h"
#include "Components/SphereComponent.h"

void UCAnimNotifyState_ApplyDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	USphereComponent* CollisionSphere = Cast<USphereComponent>(Owner->FindComponentByTag(USphereComponent::StaticClass(), FName("Attack")));
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSphere->SetGenerateOverlapEvents(true);
		CollisionSphere->RecreatePhysicsState();
	}
}

void UCAnimNotifyState_ApplyDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	USphereComponent* CollisionSphere = Cast<USphereComponent>(Owner->FindComponentByTag(USphereComponent::StaticClass(), FName("Attack")));
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionSphere->SetGenerateOverlapEvents(false);
		CollisionSphere->RecreatePhysicsState();
	}
}
