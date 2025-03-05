// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyInfectedResearcher.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"

#include "Weapon/CWeaponStructures.h"
#include "CPlayer.h"

ACEnemyInfectedResearcher::ACEnemyInfectedResearcher()
{
	SwingAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	SwingAttackCollision->SetCollisionProfileName("NoCollision");
}

void ACEnemyInfectedResearcher::EnemyAttackStart(bool bIsCloseRangeAttack)
{
	if (bIsCloseRangeAttack)
	{
		if (AttackMontages.IsValidIndex(0))
		{
			UAnimMontage* SelectedMontage = AttackMontages[0];
			if (SelectedMontage && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(SelectedMontage);
			}
		}
	}
	else
	{
		if (AttackMontages.IsValidIndex(1))
		{
			UAnimMontage* SelectedMontage = AttackMontages[1];
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
	SwingAttackCollision->OnComponentHit.AddDynamic(this, &ACEnemyInfectedResearcher::OnComponentHit);
	SwingAttackCollision->SetCollisionProfileName("NoCollision");
	if (SwingAttackCollision && GetMesh())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SwingAttackCollision->AttachToComponent(GetMesh(), AttachmentRules, FName("Rifle_hand"));
	}
}

void ACEnemyInfectedResearcher::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
		return;
	APawn* owner = Cast<APawn>(GetOwner());
	if (owner == nullptr)
		return;
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player == nullptr)
		return;
	Destroy();
	Hits.AddUnique(player);
	HitData[0].SnedDamage(owner, this, player);

}
