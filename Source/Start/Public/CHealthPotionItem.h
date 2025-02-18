// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CHealthPotionItem.generated.h"

UCLASS()
class START_API ACHealthPotionItem : public ACBaseItem
{
	GENERATED_BODY()

public:
	ACHealthPotionItem();
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(/*�÷��̾� Ŭ����*/) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float HealAmount;
};
