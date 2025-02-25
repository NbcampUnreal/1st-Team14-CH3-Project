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

// ź�� ���� ���� ��������Ʈ ���� (���� ź��, �ִ� ź��)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);

// Aim ���� ���� ��������Ʈ: true�� aim ��, false�� ����
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

public:
	FWeaponTypeChanged OnWeaponTypeChanged; // ���� Ÿ�� ���� ��������Ʈ
	FAmmoChanged OnAmmoChanged; // ź�� ���� ���� ��������Ʈ
	FAimChanged OnAimChanged; // Aim ���� ���� ��������Ʈ
	
private:
	EWeaponType Type = EWeaponType::Max;
	ACCharacter* Owner;
	TArray<ACWeapon*> Weapons;

};
