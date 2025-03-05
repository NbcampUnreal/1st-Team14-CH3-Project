// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyAIController.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CPatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACEnemyAIController::ACEnemyAIController()
{

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1200.0f;
	SightConfig->LoseSightRadius = 1400.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(UAISense_Sight::StaticClass());

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::OnPerceptionUpdated);
}

void ACEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
		return;

	ACPlayer* Player = Cast<ACPlayer>(Actor);
	ACEnemy* ControllerEnemy = Cast<ACEnemy>(GetPawn());
	if (Player && ControllerEnemy)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsBool("PlayerDetected", true);
			GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);

			ControllerEnemy->SetRun();
			ControllerEnemy->VisibleEnemyHPBar();
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool("PlayerDetected", false);

			ACPatrolPath* PatrolPath = Cast<ACPatrolPath>(GetBlackboardComponent()->GetValueAsObject("PatrolPath"));
			int CurrentWaypointIndex = GetBlackboardComponent()->GetValueAsInt("CurrentWaypointIndex");
  			GetBlackboardComponent()->SetValueAsObject("TargetActor", PatrolPath->GetWaypoint(CurrentWaypointIndex));

			ControllerEnemy->SetWalk();
			ControllerEnemy->HiddenEnemyHPBar();
		}
	}

}