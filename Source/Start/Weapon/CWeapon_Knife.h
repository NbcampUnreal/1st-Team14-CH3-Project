// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "CWeaponStructures.h"
#include "Weapon/CWeapon.h"
#include "CWeapon_Knife.generated.h"




UCLASS()
class START_API ACWeapon_Knife : public ACWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> Data;
	UPROPERTY(EditAnywhere, Category = "Collision")
	UCapsuleComponent* Collision1;
	UPROPERTY(EditAnywhere, Category = "Collision")
	UCapsuleComponent* Collision2;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* AttackSound;

public:
	ACWeapon_Knife();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginEquip() override;
	virtual void Unequip() override;
	virtual void DonAction() override;
	virtual void BeginAction() override;
	virtual void EndAction() override;

	void EnableCollision();
	void DisableCollision();

private:
	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	int32 Index;
	bool bExist;
	TArray<UCapsuleComponent*> Collisions;
	TArray<APawn*> Hits;
};

