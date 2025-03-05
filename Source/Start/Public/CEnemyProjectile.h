// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnemyProjectile.generated.h"

struct FHitData;

UCLASS(Blueprintable)
class START_API ACEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACEnemyProjectile();
	UPROPERTY(EditAnywhere, Category = "Components")
	class USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* Projectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Hit")
	TArray<FHitData> HitData;
	TArray<class ACharacter*> Hits;

private:
	FTimerHandle DeleteTimerHandle;
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void DestroyProjectile();
};
