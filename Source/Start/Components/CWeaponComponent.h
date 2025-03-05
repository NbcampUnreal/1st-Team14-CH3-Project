#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IItemInterface.h"
#include "CWeaponComponent.generated.h"

class ACWeapon;
class ACCharacter;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle, Pistol, Knife, Grenade,  Max	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimChanged, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponNameChanged, FText, NewWeaponName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAim_Arms_Begin, ACWeapon*, InThisWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAim_Arms_End);

UCLASS (ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	// 연발 사격 시 HUD 업데이트용 타이머 핸들
	FTimerHandle AmmoUpdateTimerHandle;

public:
	FORCEINLINE bool IsUnarmedMode(){return Type == EWeaponType::Max;}
	FORCEINLINE bool IsRifleMode(){return Type == EWeaponType::Rifle;}
	FORCEINLINE bool IsPistolMode(){return Type == EWeaponType::Pistol;}
	FORCEINLINE bool IsKnifeMode(){return Type == EWeaponType::Knife;}
	FORCEINLINE bool IsGrenadeMode(){return Type == EWeaponType::Grenade;}
	FORCEINLINE TArray<TSubclassOf<ACWeapon>> GetWeaponClasses() { return WeaponClasses; }
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
	UFUNCTION(BlueprintCallable)
	void SetGrenadeMode();
	void SetMode(EWeaponType InType);

private:
	void ChangeType(EWeaponType InType);

public:
	void Begin_Equip();
	void End_Equip();

	void DoAction();
	void Begin_DoAction();
	void End_DoAction();

	void Begin_Fire();
	void End_Fire();

	bool BeginAim();
	void EndAim();
	bool GetInAim();
	FVector GetLeftHandLocation();
	FVector GetLeftHandAimLocation();

	void ToggleAutoFire();
	void Reload();
	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Magazine();

	// 연발 사격 중 매 발마다 HUD 업데이트를 위한 함수
	void PollAmmoUpdate();

private:
	UFUNCTION()
	void On_Begin_Aim(ACWeapon* InThisWeapon);
	UFUNCTION()
	void On_End_Aim();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	FAmmoChanged OnAmmoChanged;
	FAimChanged OnAimChanged;
	FWeaponNameChanged OnWeaponNameChanged;
	FWeaponAim_Arms_Begin OnWeaponAim_Arms_Begin;
	FWeaponAim_Arms_End OnWeaponAim_Arms_End;
	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrentWeaponType() const { return Type; }
	int32 GetWeaponIndexFromItemType(EItemType ItemType);
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class ACWeapon>> WeaponClasses;
private:
	EWeaponType Type = EWeaponType::Max;
	ACCharacter* Owner;
	TArray<ACWeapon*> Weapons;

};
