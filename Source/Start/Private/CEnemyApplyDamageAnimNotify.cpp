// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyApplyDamageAnimNotify.h"

void UCEnemyApplyDamageAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;


}
