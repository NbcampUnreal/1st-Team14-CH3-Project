#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponComponent.generated.h"

class ACWeapon;
class ACCharacter;

UENUM(meta = (BlueprintSpawnableComponent))
enum class EWeaponType : uint8
{
	Rifle, Pistol, Knife, Max	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

// 탄약 정보 변경 델리게이트 선언 (현재 탄약, 최대 탄약)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);

// Aim 상태 변경 델리게이트: true면 aim 중, false면 해제
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimChanged, bool, bIsAiming);

UCLASS (ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category ="Settings")
	TArray<TSubclassOf<class ACWeapon>> WeaponClasses;
public:
	FORCEINLINE bool IsUnarmedModeMode(){return Type == EWeaponType::Max;}
	FORCEINLINE bool IsRifleMode(){return Type == EWeaponType::Rifle;}
	FORCEINLINE bool IsPistolMode(){return Type == EWeaponType::Pistol;}
	FORCEINLINE bool IsKnifeMode(){return Type == EWeaponType::Knife;}
public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ACWeapon* GetCurrentWeapon();
	
public:
	void SetUnarmedMode();
	void SetPistolMode();
	void SetRifleMode();
	void SetKnifeMode();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	void Begin_Equip();
	void End_Equip();

	void Begin_Fire();
	void End_Fire();

UFUNCTION(BlueprintCallable)
	void BeginAim();
UFUNCTION(BlueprintCallable)
	void EndAim();

	FVector GetLefttHandLocation();
UFUNCTION(BlueprintCallable)
	void ToggleAutoFire();
UFUNCTION(BlueprintCallable)
	void Reload();

public:
	FWeaponTypeChanged OnWeaponTypeChanged; // 무기 타입 변경 델리게이트
	FAmmoChanged OnAmmoChanged; // 탄약 정보 변경 델리게이트
	FAimChanged OnAimChanged; // Aim 상태 변경 델리게이트
	
private:
	EWeaponType Type = EWeaponType::Max;
	ACCharacter* Owner;
	TArray<ACWeapon*> Weapons;

};
