// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestHuman.generated.h"

class CItemInventory;

UCLASS()
class START_API ATestHuman : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestHuman();
	TSharedPtr< CItemInventory> GetInventory();
	int health = 50;

protected:
	virtual void BeginPlay() override;

	class TSharedPtr< CItemInventory> ItemInventory;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

};
