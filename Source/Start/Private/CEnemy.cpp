// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include "CEnemyAIController.h"
#include "Perception/AISense_Damage.h"
#include "CGameInstance.h"
#include "BrainComponent.h"
#include "CSpawnComponent.h"

#include "Blueprint/UserWidget.h"

#include "CGameState.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CPlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

ACEnemy::ACEnemy()
{

	UE_LOG(LogTemp, Warning, TEXT("CEnemy Constructor Called"));

	AIControllerClass = ACEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	OverheadHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadHP Widget"));
	OverheadHPWidget->SetupAttachment(GetMesh());
	OverheadHPWidget->SetWidgetSpace(EWidgetSpace::Screen);

	SpawnComp = CreateDefaultSubobject<UCSpawnComponent>(TEXT("SpawnComp"));


	BossHPWidgetClass = nullptr;
	BossHPWidgetInstance = nullptr;

	bCanAttack = false;
	bIsGunUsed = false;
	bIsDied = false;
}

void ACEnemy::UpdateOverheadHP()
{
	if (BossHPWidgetInstance)
	{
		if (UProgressBar* HPBar = Cast<UProgressBar>(BossHPWidgetInstance->GetWidgetFromName(TEXT("HPBar"))))
		{
			HPBar->SetPercent(StatusComponent->GetHealth() / StatusComponent->GetMaxHealth());
		}
		if (UTextBlock* BossName = Cast<UTextBlock>(BossHPWidgetInstance->GetWidgetFromName(TEXT("EnemyName"))))
		{
			BossName->SetText(EnemyName);
		}
	}

	if (OverheadHPWidget)
	{
		UUserWidget* OverheadHPWidgetInstance = OverheadHPWidget->GetUserWidgetObject();
		if (!OverheadHPWidgetInstance) return;

		if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadHPWidgetInstance->GetWidgetFromName(TEXT("HP"))))
		{
			HPBar->SetPercent(StatusComponent->GetHealth() / StatusComponent->GetMaxHealth());
		}
	}


}



float ACEnemy::GetEnemyHP() const
{
	return StatusComponent->GetHealth();
}

void ACEnemy::VisibleEnemyHPBar()
{
	if (BossHPWidgetClass == nullptr)
	{
		return;
	}

	if (!BossHPWidgetInstance)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			ACPlayerController* CPlayerController = Cast<ACPlayerController>(PlayerController);
			if (CPlayerController)
			{
				BossHPWidgetInstance = CreateWidget<UUserWidget>(CPlayerController, BossHPWidgetClass);
				BossHPWidgetInstance->AddToViewport();
			}
		}

		UpdateOverheadHP();
	}

}

void ACEnemy::HiddenEnemyHPBar() 
{
	if (BossHPWidgetInstance)
	{
		BossHPWidgetInstance->RemoveFromParent();
		BossHPWidgetInstance = nullptr;
	}
}

void ACEnemy::EnemyAttackStart()
{

}

void ACEnemy::EnemyAttackEnd()
{

}

void ACEnemy::SetIdleMode()
{
	StateComponent->SetIdleMode();
}

void ACEnemy::SetEquipMode()
{
	StateComponent->SetEquipMode();
}

void ACEnemy::SetActionMode()
{
	StateComponent->SetActionMode();
}

bool ACEnemy::IsEnemyActionMode()
{
	return StateComponent->IsActionMode();
}

void ACEnemy::SetRun()
{
	MovementComponent->OnRun();
}

void ACEnemy::SetWalk()
{
	MovementComponent->OnWark();
}

void ACEnemy::SetStun(ACPlayer* Player)
{
	UCMovementComponent* PlayerMovementComponent = Player->FindComponentByClass<UCMovementComponent>();
	if (PlayerMovementComponent->GetCanMove())
	{
		
		if (PlayerMovementComponent)
		{
			PlayerMovementComponent->Stop();
		}
		FTimerHandle StunTimerHandle;
		GetWorldTimerManager().SetTimer(StunTimerHandle, [PlayerMovementComponent]()
			{
				if (PlayerMovementComponent)
				{
					PlayerMovementComponent->Move();
				}
			}, 3.0f, false);
	}

}



void ACEnemy::SpawnRandomItemAfterDie()
{
	SpawnComp->SpawnRandomActorToPoint(GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);

}

void ACEnemy::ToDoAfterDie()
{
	Destroy();
	SpawnRandomItemAfterDie();
}

void ACEnemy::Die()
{
	Super::Die();

	bIsDied = true;

	ACEnemyAIController* AIController = Cast<ACEnemyAIController>(GetController());
	if (AIController&& AIController->BrainComponent)
	{
		AIController->BrainComponent->StopLogic(TEXT("Character Died"));
		//AIController->StopMovement();
	}
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCGameInstance* CGameInstance = Cast<UCGameInstance>(GameInstance);
		if (CGameInstance)
		{
			CGameInstance->AddScore(StatusComponent->GetMaxHealth()/2);
		}
	}


	GetWorldTimerManager().SetTimer(DieTimerHandle, this, &ACEnemy::ToDoAfterDie, 3.0f, false);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (CameraManager)
	{
		UWidgetComponent* HPBarWidgetComponent = FindComponentByClass<UWidgetComponent>();
		if (HPBarWidgetComponent)
		{
			HPBarWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(HPBarWidgetComponent->GetComponentLocation(), CameraManager->GetCameraLocation()));
		}
	}
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TempDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		DamageCauser,
		DamageAmount,
		GetActorLocation(),
		GetActorLocation()
	);

	UpdateOverheadHP();
	return TempDamageAmount;
}

