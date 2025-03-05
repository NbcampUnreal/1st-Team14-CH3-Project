// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
public:
	void DoAction(class ACharacter* Owners);
};

USTRUCT()
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
	void SnedDamage(APawn* InAttacker, AActor* InAttackCauser, APawn* InOther);
	void PlayMontage(ACharacter* InOwner);
	void PlaySound(ACharacter* InOwner);
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()
public:
	FHitData* HitData;
};

UCLASS()
class START_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()

};
