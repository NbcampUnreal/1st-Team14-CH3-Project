#include "Components/CWeaponComponent.h"
#include "CCharacter.h"
#include "Weapon/CWeapon.h"

UCWeaponComponent::UCWeaponComponent()
{
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACCharacter>(GetOwner());
	if (Owner == nullptr)
		return;

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (TSubclassOf<ACWeapon> weaponClass : WeaponClasses)
	{
		if (weaponClass != nullptr)
		{
			ACWeapon* weapon = Owner->GetWorld()->SpawnActor<ACWeapon>(weaponClass,params);
			Weapons.Add(weapon);
		}
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{
	if (IsUnarmedModeMode() == true)
		return nullptr;
	return Weapons[(int32)Type];
}

void UCWeaponComponent::SetUnarmedMode()
{
	if (GetCurrentWeapon()->CanUnequip() == false)
		return;
	GetCurrentWeapon()->Unequip();
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetRifleMode()
{
	SetMode(EWeaponType::Rifle);
}

void UCWeaponComponent::SetKnifeMode()
{
	SetMode(EWeaponType::Knife);
}

void UCWeaponComponent::SetPistolMode()
{
	SetMode(EWeaponType::Pistol);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		
		return;
	}
	else if(IsUnarmedModeMode() == false)
	{
		//무기를 장착하고 있는 상태라면 현재 무기를 장착해제할 수 있는지 체크한뒤 무기 장착 해제
		if(GetCurrentWeapon()->CanUnequip() == false)
			return;

		GetCurrentWeapon()->Unequip();
	}

	if (Weapons[(int32)InType] == nullptr)
		return;
	if (Weapons[(int32)InType]->CanEquip() == false)
		return;
	Weapons[(int32)InType]->Equip();
	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, Type);
}

void UCWeaponComponent::Begin_Equip()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->BeginEquip();
}

void UCWeaponComponent::End_Equip()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->EndEquip();
}

void UCWeaponComponent::Begin_Fire()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	if (GetCurrentWeapon()->CanFire() == false)
		return;

	GetCurrentWeapon()->BeginFire();
}

void UCWeaponComponent::End_Fire()
{
	if (GetCurrentWeapon() == nullptr)
		return;;

	GetCurrentWeapon()->EndFire();
}

void UCWeaponComponent::BeginAim()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	//GetCurrentWeapon()->BeginAim();
}

void UCWeaponComponent::EndAim()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	//GetCurrentWeapon()->EndAim();
}

FVector UCWeaponComponent::GetLefttHandLocation()
{
	if (GetCurrentWeapon() == nullptr)
		return FVector::Zero();

	return GetCurrentWeapon()->GetLeftHandLocation();
}

void UCWeaponComponent::ToggleAutoFire()
{
	if(GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->ToggleAutoFire();
}

