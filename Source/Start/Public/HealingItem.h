// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class START_API AHealingItem : public ACBaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(/*플레이어 클래스*/) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float HealAmount;
};
