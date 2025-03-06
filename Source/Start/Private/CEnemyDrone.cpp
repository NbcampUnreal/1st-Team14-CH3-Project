// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyDrone.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ACEnemyDrone::ACEnemyDrone() : Super()
{
    bIsGunUsed = true;
}

void ACEnemyDrone::LaserSpawnAttack()
{
    if (!LaserActor)
        return;

    FTransform GunL = GetMesh()->GetSocketTransform("gunL");
    GunL.SetScale3D(FVector(1, 1, 1));
    FTransform GunR = GetMesh()->GetSocketTransform("gunR");
    GunR.SetScale3D(FVector(1, 1, 1));
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    GetWorld()->SpawnActor<AActor>(LaserActor, GunL, SpawnParams);
    GetWorld()->SpawnActor<AActor>(LaserActor, GunR, SpawnParams);
    

    if (LaserSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaserSound, GetActorLocation());
    }
}

void ACEnemyDrone::EnemyAttackStart(bool bIsCloseRangeAttack)
{
    Super::EnemyAttackStart(bIsCloseRangeAttack);
    LaserSpawnAttack();
}

void ACEnemyDrone::EnemyAttackEnd()
{
    Super::EnemyAttackEnd();
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
