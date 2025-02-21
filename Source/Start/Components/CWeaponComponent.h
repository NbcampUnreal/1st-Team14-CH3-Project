#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponComponent.generated.h"

class ACCharacter;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle, Pistol, Knife, Max	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
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

private:
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

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	
private:
	EWeaponType Type = EWeaponType::Max;
	ACCharacter* Owner;
	TArray<ACWeapon*> Weapons;

};
