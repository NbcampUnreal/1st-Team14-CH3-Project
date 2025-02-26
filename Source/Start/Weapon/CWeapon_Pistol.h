// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CWeapon.h"
#include "CWeapon_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class START_API ACWeapon_Pistol : public ACWeapon
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "FIre")
	UAnimMontage* Montage;
public:
	ACWeapon_Pistol();
};
