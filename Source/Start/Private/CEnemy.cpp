// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include "CEnemyAIController.h"
#include "Weapon/CWeapon.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"
#include "CGameState.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

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
		HPBar->SetPercent(StatusComponent->GetHealth() /StatusComponent->GetMaxHealth());
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

void ACEnemy::SetEquipMode()
{
	StateComponent->SetEquipMode();
}

void ACEnemy::SetActionMode()
{
	StateComponent->SetActionMode();
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

void ACEnemy::Equip()
{
	if (!bIsEqueped)
	{
		WeaponComponent->SetRifleMode();
		bIsEqueped = true;
	}
}

void ACEnemy::UnEquip()
{
	if(bIsEqueped)
	{
		WeaponComponent->SetUnarmedMode();
		bIsEqueped = false;
	}
}

void ACEnemy::ToggleAutoFire()
{
	if (ACWeapon* Weapon = WeaponComponent->GetCurrentWeapon())
	{
		Weapon->ToggleAutoFire();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Toggle")));
	}
		
}

void ACEnemy::GunAttackStart()
{
	WeaponComponent->Begin_Fire();
}

void ACEnemy::GunAttackEnd()
{
	WeaponComponent->End_Fire();
}

void ACEnemy::DestroyEnemyInMaze()
{
	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (GameState)
	{
		EGameState EGS = GameState->GetGameState();

		if (EGS == EGameState::Labyrinth)
		{
			Destroy();
		}
	}
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bCanAttack = StateComponent->IsActionMode();

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

	UpdateOverheadHP();
	return TempDamageAmount;
}
