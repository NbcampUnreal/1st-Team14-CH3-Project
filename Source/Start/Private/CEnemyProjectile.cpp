// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemyProjectile.h"
#include "CPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
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
	BoxComp->OnComponentHit.AddDynamic(this, &ACEnemyProjectile::OnComponentHit);
	
	//GetWorldTimerManager().SetTimer(DeleteTimerHandle, this, &ACEnemyProjectile::DestroyProjectile, 0.3f, false);

}

void ACEnemyProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
		return;
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player == nullptr)
		return;

	Hits.AddUnique(player);
	HitData[0].SnedDamage(nullptr, this, player);
}

void ACEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

