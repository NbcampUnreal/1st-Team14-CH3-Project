// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyProjectile.h"
#include "CPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

#include "Weapon/CWeaponStructures.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACEnemyProjectile::ACEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(StaticMeshComp);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
}

// Called when the game starts or when spawned
void ACEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACEnemyProjectile::OnComponentBeginOverlap);
	
	GetWorldTimerManager().SetTimer(DeleteTimerHandle, this, &ACEnemyProjectile::DestroyProjectile, 0.3f, false);

}

void ACEnemyProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
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

void ACEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

