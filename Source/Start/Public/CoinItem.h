// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class START_API ACoinItem : public ACBaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 PointValue;

	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(/*플레이어 클래스*/) override;
};
