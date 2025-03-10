// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CEnemy.h"
#include "CEnemyDrone.generated.h"


UCLASS()
class START_API ACEnemyDrone : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemyDrone();
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> LaserActor;
	UPROPERTY(EditAnywhere, Category = "Attack")
	USoundBase* LaserSound;

	void LaserSpawnAttack();
	virtual void EnemyAttackStart(bool bIsCloseRangeAttack) override;
	virtual void EnemyAttackEnd() override;
	virtual void Die() override;
};
