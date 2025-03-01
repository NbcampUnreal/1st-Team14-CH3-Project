// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CStateInteractionActor.generated.h"

UCLASS()
class START_API ACStateInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACStateInteractionActor();

protected:


public:	
	virtual void Tick(float DeltaTime) override;

};
