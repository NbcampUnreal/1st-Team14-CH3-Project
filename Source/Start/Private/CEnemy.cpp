// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include "CEnemyAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

ACEnemy::ACEnemy()
{
	AIControllerClass = ACEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	OverheadHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadHP Widget"));
	OverheadHPWidget->SetupAttachment(GetMesh());
	OverheadHPWidget->SetWidgetSpace(EWidgetSpace::Screen);

	bCanAttack = false;
	bIsGunUsed = false;
}

void ACEnemy::UpdateOverheadHP()
{
	if (!OverheadHPWidget) return;

	UUserWidget* OverheadHPWidgetInstance = OverheadHPWidget->GetUserWidgetObject();
	if (!OverheadHPWidgetInstance) return;

	if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadHPWidgetInstance->GetWidgetFromName(TEXT("HP"))))
	{
		HPBar->SetPercent(GetHealth() / GetMaxHealth());
	}

}

void ACEnemy::EnemyAttack()
{
	StateComponent->SetActionMode();

	OnEnemyAttack.Broadcast();
}

void ACEnemy::SetIdleMode()
{
	StateComponent->SetIdleMode();
}

void ACEnemy::SetRun()
{
	MovementComponent->OnRun();
}

void ACEnemy::SetWalk()
{
	MovementComponent->OnWark();
}


void ACEnemy::Tick(float DeltaTime)
{
	bCanAttack = StateComponent->IsActionMode();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("%d"), bCanAttack));
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TempDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UpdateOverheadHP();
	return TempDamageAmount;
}
