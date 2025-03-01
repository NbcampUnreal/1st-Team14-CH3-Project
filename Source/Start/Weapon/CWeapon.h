#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CWeapon.generated.h"

class UCWeaponComponent;
class UMaterialInstanceConstant;
class ACMagazine;
class ACBullet;
class UCCameraComponent;
class UCStateComponent;
class ACCharacter;
class UTimelineComponent;

UENUM(BlueprintType)
enum class EGunType : uint8
{
	AK, AR4, Shotgun, Pistol, Knife, Max
};

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
	void SetData(class ACCharacter* InOwner);
	void SetDataByNoneCurve(class ACCharacter* InOwner);
};

UCLASS(Abstract)
class START_API ACWeapon : public ACBaseItem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EGunType GunType = EGunType::Max;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSokcetName;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName FistHandSokcetName;//주먹이 있을때만 그클래스에서 충돌체 만들어서 사용
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandAimSokcetName;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandAimLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FTransform WeapoLeftHandTransform;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float Equip_PlayRate;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	USoundWave* EquipSound;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	USoundWave* UnequipSound;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	UStaticMeshComponent* SightMesh;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	bool bIsCustom;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	FWeaponAimData BaseData;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	FWeaponAimData AimData;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	UCurveFloat* AimCurve;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	float AimSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, category = "Aim")
	USoundWave* BreathSound;

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
	TSubclassOf<UCameraShakeBase> AimCameraShak;
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
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	FName MagazineBoneName;
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	TSubclassOf<ACMagazine> MagazineClass;
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	FName MagazinSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	USoundBase* ReloadSound;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Arms")
	FTransform ArmsMeshTransform;
	UPROPERTY(EditDefaultsOnly, Category = "Arms")
	FTransform ArmsLeftHandTransform;

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> Debug;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	FLinearColor DebugColor = FColor::Red;

private:
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* Timeline;


protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FText WeaponDisplayName;

public:
	FORCEINLINE bool IsAutoFire() { return bAutoFire; }
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }
	FORCEINLINE FVector GetLeftHandAimLocation() { return LeftHandAimLocation; }
	FORCEINLINE FTransform GetWeaponLeftHandAimTransform() { return WeapoLeftHandTransform; }

	FORCEINLINE bool GetInAim() const { return bInAim; }

	FORCEINLINE uint8 GetCurrentMagazineCount() const { return CurrentMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() const { return MaxMagazineCount; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FText GetWeaponDisplayName() const { return WeaponDisplayName; }

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	FORCEINLINE EGunType GetGunType() { return GunType; }


public:
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	virtual void BeginEquip();
	virtual void EndEquip();

	bool CanUnequip();
	virtual void Unequip();

public:
	virtual void DonAction();
	virtual void BeginAction();
	virtual void EndAction();

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
	virtual void BeginAim();
	virtual void EndAim();

private:
	UFUNCTION()
	void OnAiming(float Output);

	UFUNCTION()
	void OnBullet(AActor* InCauser, ACharacter* InOtherCharacter);

protected:
	ACCharacter* OwnerCharacter;
	UCStateComponent* State;
	UCCameraComponent* Camera;
	UCWeaponComponent* Weapon;

private:
	FTimerHandle AutoFireHandle;
	bool bEquipping;
	bool bInAim;
	bool bFiring;
	bool bAutoFire = true;

protected:
	bool bReload;

	ACMagazine* Magazine;

protected:
	uint8 CurrentMagazineCount;
	bool bBeginAction;
	bool bEnable;

};
