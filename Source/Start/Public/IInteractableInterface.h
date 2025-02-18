// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class START_API IIInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnInteractableOverlap(
		UPrimitiveComponent* OverlapPrimitiveComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnInteractableEndOverlap(
		UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

	virtual void OverlapActivate(AActor* Activator) = 0;
};
