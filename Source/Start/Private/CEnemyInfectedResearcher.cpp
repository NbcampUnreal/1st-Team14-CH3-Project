// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyInfectedResearcher.h"
#include "Animation/AnimInstance.h"
#include "Components/CStatusComponent.h"
#include "Components/SphereComponent.h"

#include "Weapon/CWeaponStructures.h"
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%d"), bIsCloseRangeAttack));
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
			else if (Phase == 2 && bIsBoss && LongRangeAttackMontages[2])
			{
				SelectedMontage = LongRangeAttackMontages[2];
			}
			
			if (SelectedMontage && GetMesh()->GetAnimInstance())
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
		if (StatusComponent->GetHealth() <= StatusComponent->GetMaxHealth() * 0.7f && Phase < 1)
		{
			Phase = 1;
			if(ChangePhaseMontages.Num()>0)
				GetMesh()->GetAnimInstance()->Montage_Play(ChangePhaseMontages[0]);
		}
		else if (StatusComponent->GetHealth() <= StatusComponent->GetMaxHealth() * 0.4f && Phase < 2)
		{
			Phase = 2;
			if (ChangePhaseMontages.Num() > 0)
				GetMesh()->GetAnimInstance()->Montage_Play(ChangePhaseMontages[1]);
		}
	}
}