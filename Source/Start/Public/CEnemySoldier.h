// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CEnemy.h"
#include "CEnemySoldier.generated.h"

/**
 * 
 */
UCLASS()
class START_API ACEnemySoldier : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemySoldier();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEqueped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldOneFire;

	UFUNCTION(BlueprintCallable)
	void Equip();

	UFUNCTION(BlueprintCallable)
	void UnEquip();

	UFUNCTION(BlueprintCallable)
	void ToggleAutoFire();

	UFUNCTION(BlueprintCallable)
	void GunAttackStart();

	UFUNCTION(BlueprintCallable)
	void GunAttackEnd();

	virtual void BeginPlay() override;
	virtual void EnemyAttackStart() override;
	virtual void EnemyAttackEnd() override;
};
