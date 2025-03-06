// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CDamageType.generated.h"

/**
 * 
 */
UCLASS()
class START_API UCDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UCDamageType();
public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
	UPROPERTY(EditAnywhere)
	float Power;
	UPROPERTY(EditAnywhere)
	USoundWave* Sound;
};
