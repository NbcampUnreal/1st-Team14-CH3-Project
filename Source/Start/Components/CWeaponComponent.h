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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimChanged, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponNameChanged, FText, NewWeaponName);

UCLASS (ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category ="Settings")
	TArray<TSubclassOf<class ACWeapon>> WeaponClasses;

	// 연발 사격 시 HUD 업데이트용 타이머 핸들
	FTimerHandle AmmoUpdateTimerHandle;

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

	void BeginAim();
	void EndAim();
	bool GetInAim();
	FVector GetLefttHandLocation();
	FVector GetLefttHandAimLocation();

	void ToggleAutoFire();
	void Reload();
	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Magazine();

	// 연발 사격 중 매 발마다 HUD 업데이트를 위한 함수
	void PollAmmoUpdate();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	FAmmoChanged OnAmmoChanged;
	FAimChanged OnAimChanged;
	FWeaponNameChanged OnWeaponNameChanged;
	
private:
	EWeaponType Type = EWeaponType::Max;
	ACCharacter* Owner;
	TArray<ACWeapon*> Weapons;

};
