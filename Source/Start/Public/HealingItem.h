// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class START_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();
	virtual void OverlapActivate(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float HealAmount;
};
