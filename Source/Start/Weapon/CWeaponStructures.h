// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "CWeaponStructures.generated.h"

USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
	UPROPERTY(EditAnywhere)
	float Power;
	UPROPERTY(EditAnywhere)
	USoundWave* Sound;

public:
	void SnedDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);
	void PlayMontage(ACharacter* InOwner);
	void PlaySound(ACharacter* InOwner);
};

USTRUCT(BlueprintType)
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()
public:
	FHitData* HitData;
};

UCLASS()
class START_API ACWeaponStructures : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeaponStructures();

protected:
	virtual void BeginPlay() override;
};
