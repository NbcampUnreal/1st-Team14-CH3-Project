// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

class ACCharacter;

UCLASS(Abstract)
class START_API ACWeapon : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSokcetName;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float Equip_PlayRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool Debug = false;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	FColor DebugColor = FColor::Red;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bPersistentLine = true;
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void BeginEquip();
	void EndEquip();

	bool CanUnequip();
	void Unequip();

public:
	bool CanFire();
	void BeginFire();
	void EndFire();
private:
	UFUNCTION()
	void OnFireing();

private:
	ACCharacter* OwnerCharacter;

	bool bEquipping;
	bool bInAim;
	bool bFiring;
	bool bReload;
	bool bAutoFire = true;
};
