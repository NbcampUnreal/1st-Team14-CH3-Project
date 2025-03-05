// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CEnemy.h"
#include "CEnemyInfectedResearcher.generated.h"

struct FHitData;

UCLASS()
class START_API ACEnemyInfectedResearcher : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemyInfectedResearcher();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animations")
	TArray<UAnimMontage*> AttackMontages;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* SwingAttackCollision;

	UPROPERTY(EditAnywhere, Category = "Hit")
	TArray<FHitData> HitData;
	TArray<class ACharacter*> Hits;
	virtual void EnemyAttackStart(bool bIsCloseRangeAttack) override;
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
