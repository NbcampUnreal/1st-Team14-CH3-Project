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
	UFUNCTION(BlueprintCallable)
	void SetCloseRangeAttack(bool IsClose) { bIsCloseRangeAttack = IsClose; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animations")
	TArray<UAnimMontage*> CloseRangeAttackMontages;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animations")
	TArray<UAnimMontage*> LongRangeAttackMontages;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* SwingAttackCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsCloseRangeAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBoss;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hit")
	int Phase;

	UPROPERTY(EditAnywhere, Category = "Hit")
	TArray<FHitData> HitData;
	TArray<class ACharacter*> Hits;
	virtual void EnemyAttackStart(bool bIsCloseAttack) override;
	virtual void BeginPlay() override;


private:
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
