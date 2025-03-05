// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyInfectedResearcher.h"
#include "Animation/AnimInstance.h"
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
			int32 Index = FMath::RandRange(0, LongRangeAttackMontages.Num() - 1);
			UAnimMontage* SelectedMontage = LongRangeAttackMontages[Index];
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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("CloseAttack")));
	}
	else
	{
		HitData[1].SnedDamage(this, this, player);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("LongAttack")));
	}
	
}
