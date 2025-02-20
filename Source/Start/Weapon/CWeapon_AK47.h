// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CWeapon.h"
#include "CWeapon_AK47.generated.h"

/**
 * 
 */
UCLASS()
class START_API ACWeapon_AK47 : public ACWeapon
{
	GENERATED_BODY()
public:
	ACWeapon_AK47();
protected:
	virtual void BeginPlay() override;
};
