// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

class UCCameraComponent;
class UCStateComponent;
class ACCharacter;
class UTimelineComponent;

USTRUCT()
struct FWeaponAimData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength;
	UPROPERTY(EditAnywhere)
	FVector SocketOffset;
	UPROPERTY(EditAnywhere)
	float FieldOfView;

public:
	void SetData(class ACCharacter*  InOwner);
	void SetDataByNoneCurve(class ACCharacter*  InOwner);
};

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

	//UPROPERTY(EditDefaultsOnly, category = "Aim")
	//FWeaponAimData BaseData;
	//UPROPERTY(EditDefaultsOnly, category = "Aim")
	//FWeaponAimData AimData;
	//UPROPERTY(EditDefaultsOnly, category = "Aim")
	//UCurveFloat* AimCurve;
	//UPROPERTY(EditDefaultsOnly, category = "Aim")
	//float AimSpeed = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;

private:
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
	UTimelineComponent* Timeline;
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UCStateComponent* State;
	UPROPERTY(VisibleAnywhere)
	UCCameraComponent* Camera;

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
public:
/*	bool CanAim();
	void BeginAim();
	void EndAim();*/

private:
	/*UFUNCTION()
	void OnAiming(float Output);*/

protected:
	ACCharacter* OwnerCharacter;
private:

	bool bEquipping;
	bool bInAim;
	bool bFiring;
	bool bReload;
	bool bAutoFire = true;
};
