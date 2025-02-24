// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CWeapon.generated.h"

class ACBullet;
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
	UPROPERTY(VisibleAnywhere)
	bool bEnableCameraLag;
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
	FVector LeftHandLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float Equip_PlayRate;

	UPROPERTY(EditDefaultsOnly, category = "Aim")
	FWeaponAimData BaseData;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	FWeaponAimData AimData;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	UCurveFloat* AimCurve;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	float AimSpeed = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UMaterialInstanceConstant* HitDecal;
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	FVector HitDecalSize = FVector(5);
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDecalLifeTime = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UAnimMontage* FireMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireRate;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UParticleSystem* FlashParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UParticleSystem* EjectParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	USoundWave* FireSound;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilAngle;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<UCameraShakeBase> CameraShak;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float AutoFireInterval;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilRate;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<ACBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine;")
	uint8 MaxMagazineCount;
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	float ReloadPlayRate;
private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> Debug;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	FLinearColor DebugColor = FColor::Red;
	
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
	FORCEINLINE bool IsAutoFire() { return bAutoFire; }
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

	// 탄약 정보를 HUD에 제공하기 위한 getter 함수들
	FORCEINLINE uint8 GetCurrentMagazineCount() const { return CurrentMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() const { return MaxMagazineCount; }
	// 편의상 HUD 연동용 별칭 함수 추가
	FORCEINLINE uint8 GetMaxAmmo() const { return GetMaxMagazineCount(); }
	FORCEINLINE uint8 GetCurrentAmmo() const { return GetCurrentMagazineCount(); }

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
	void ToggleAutoFire();

	bool CanReload();
	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Magazine();
public:
	bool CanAim();
	void BeginAim();
	void EndAim();

private:
	UFUNCTION()
	void OnAiming(float Output);

protected:
	ACCharacter* OwnerCharacter;
private:
	FTimerHandle AutoFireHandle;
	bool bEquipping;
	bool bInAim;
	bool bFiring;
	bool bReload;
	bool bAutoFire = true;

protected:
	uint8 CurrentMagazineCount;
};
