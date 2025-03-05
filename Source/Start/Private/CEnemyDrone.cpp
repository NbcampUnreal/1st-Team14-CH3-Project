// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyDrone.h"
#include "Components/CapsuleComponent.h"

ACEnemyDrone::ACEnemyDrone() : Super()
{
    bIsGunUsed = true;
}

void ACEnemyDrone::EnemyAttackStart()
{
    SetbCanAttack(true);
}

void ACEnemyDrone::EnemyAttackEnd()
{
    SetbCanAttack(false);
}

void ACEnemyDrone::Die()
{
    Super::Die();

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCapsuleHalfHeight(24, true);
    }
}
