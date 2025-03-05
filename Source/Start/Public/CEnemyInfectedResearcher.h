// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CEnemy.h"
#include "CEnemyInfectedResearcher.generated.h"

UCLASS()
class START_API ACEnemyInfectedResearcher : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemyInfectedResearcher();
protected:
	virtual void EnemyAttackStart() override;
	virtual void EnemyAttackEnd() override;
};
