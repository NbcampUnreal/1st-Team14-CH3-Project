// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CWeapon.h"
#include "CWeapon.h"
#include "CWeapon_Rifle.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class START_API ACWeapon_Rifle : public ACWeapon
{
	GENERATED_BODY()

public:
	ACWeapon_Rifle();
protected:
	virtual void BeginPlay() override;
public:
	virtual void BeginAim() override;
	virtual void EndAim() override;
};