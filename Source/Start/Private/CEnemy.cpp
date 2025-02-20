// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include "CEnemyAIController.h"

ACEnemy::ACEnemy()
{
	AIControllerClass = ACEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
