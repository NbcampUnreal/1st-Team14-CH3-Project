// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CInteractable.h"
#include "CLevelMover.generated.h"

/**
 * 
 */
UCLASS()
class START_API ACLevelMover : public ACInteractable
{
	GENERATED_BODY()

public:
	ACLevelMover();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelMover")
	TSoftObjectPtr<UWorld> LevelToMove;

	virtual void KeyPressedActivate(AActor* Activator) override;
	
};
