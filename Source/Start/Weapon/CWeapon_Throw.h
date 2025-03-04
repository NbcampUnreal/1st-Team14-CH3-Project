// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CWeapon.h"
#include "CWeapon_Throw.generated.h"

class ACGrenadesItem;
/**
 * 
 */
UCLASS()
class START_API ACWeapon_Throw : public ACWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TSubclassOf<ACGrenadesItem> GrenadesClass;
public:
	ACWeapon_Throw();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginEquip() override;
	virtual void Unequip() override;
	virtual void DonAction() override;
	virtual void BeginAction() override;
	virtual void EndAction() override;
	
};
