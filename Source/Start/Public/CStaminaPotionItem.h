// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CStaminaPotionItem.generated.h"

UCLASS()
class START_API ACStaminaPotionItem : public ACBaseItem
{
	GENERATED_BODY()
public:
	ACStaminaPotionItem();

protected:
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(AActor* Target) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float StaminaRecoveryAmount;
};
