// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyInfectedResearcher.h"
#include "Animation/AnimInstance.h"
#include "Components/CStatusComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Weapon/CWeaponStructures.h"
#include "CSimbioComponent.h"
#include "CPlayer.h"

ACEnemyInfectedResearcher::ACEnemyInfectedResearcher() :
	bIsCloseRangeAttack(false),
	bIsBoss(false),
	Phase(0)
{
	SwingAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	SwingAttackCollision->SetCollisionProfileName("NoCollision");
	SwingAttackCollision->SetupAttachment(GetMesh());



}

void ACEnemyInfectedResearcher::EnemyAttackStart(bool bIsCloseAttack)
{
	Super::EnemyAttackStart(bIsCloseRangeAttack);
	if (bIsCloseAttack)
	{
		bIsCloseRangeAttack = true;
		if (CloseRangeAttackMontages.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, CloseRangeAttackMontages.Num() - 1);
			UAnimMontage* SelectedMontage = CloseRangeAttackMontages[Index];
			if (SelectedMontage && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(SelectedMontage);
			}
		}
	}
	else
	{
		bIsCloseRangeAttack = false;
		if (LongRangeAttackMontages.Num() > 0)
		{
			UAnimMontage* SelectedMontage = nullptr;
			if (Phase == 0)
			{
				SelectedMontage = LongRangeAttackMontages[0];
			}
			else if (Phase == 1 && bIsBoss && LongRangeAttackMontages[1])
			{
				SelectedMontage = LongRangeAttackMontages[1];
			}
			else if (Phase == 2)
			{
				if(!SimbioComponent->GetIsSimbioActivate())
					SimbioComponent->ActivateSimbio();
				SimbioComponent->SimbioAttack();
			}
			
			if (SelectedMontage && GetMesh()->GetAnimInstance()&& Phase != 2)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(SelectedMontage);
			}
		}
	}
}

void ACEnemyInfectedResearcher::BeginPlay()
{
	Super::BeginPlay();
	SwingAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemyInfectedResearcher::OnComponentBeginOverlap);
	SwingAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SimbioComponent->DeActivateSimbio();
}

void ACEnemyInfectedResearcher::UpdateOverheadHP()
{
	Super::UpdateOverheadHP();

	if (!bIsBoss)
		return;

	if (Phase == 1)
	{
		if (BossHPWidgetInstance)
		{
			if (UProgressBar* HPBar = Cast<UProgressBar>(BossHPWidgetInstance->GetWidgetFromName(TEXT("HPBar"))))
			{
				HPBar->SetFillColorAndOpacity(FColor(148, 0, 0));
			}
		}
	}
	else if(Phase == 2)
	{
		if (BossHPWidgetInstance)
		{
			if (UProgressBar* HPBar = Cast<UProgressBar>(BossHPWidgetInstance->GetWidgetFromName(TEXT("HPBar"))))
			{
				HPBar->SetFillColorAndOpacity(FColor(49, 0, 0));
			}
		}
	}
}

void ACEnemyInfectedResearcher::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == nullptr)
		return;
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player == nullptr)
		return;
	Hits.AddUnique(player);
	if (bIsCloseRangeAttack)
	{
		HitData[0].SnedDamage(this, this, player);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("CloseAttack")));
	}
	else
	{
		HitData[1].SnedDamage(this, this, player);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("LongAttack")));
	}
	
}

float ACEnemyInfectedResearcher::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	float TempDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangePhase();

	return TempDamageAmount;
}

void ACEnemyInfectedResearcher::ChangePhase()
{
	if (bIsBoss)
	{
		if (StatusComponent->GetHealth() <= StatusComponent->GetMaxHealth() * 0.8f && Phase < 1)
		{
			Phase = 1;
			if(ChangePhaseMontages.Num()>0)
				GetMesh()->GetAnimInstance()->Montage_Play(ChangePhaseMontages[0]);
		}
		else if (StatusComponent->GetHealth() <= StatusComponent->GetMaxHealth() * 0.5f && Phase < 2)
		{
			Phase = 2;
			if (ChangePhaseMontages.Num() > 0)
				GetMesh()->GetAnimInstance()->Montage_Play(ChangePhaseMontages[1]);
		}
	}
}