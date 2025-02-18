#include "Components/CWeaponComponent.h"

//#include "Characters/CCharacter.h"
#include "Weapon/CWeapon.h"

UCWeaponComponent::UCWeaponComponent()
{
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	/*Owner = Cast<ACCharacter>(GetOwner());
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
	}*/
}

void UCWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{
	if (IsKnifeMode() == true)
		return nullptr;
	return Weapons[(int32)Type];
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
	if (Type == InType)//현재 무기와 장착하려는 무기 같을 때 무기 장착 해제
		return;
	
	else if(IsKnifeMode() == false)
	{
		//무기를 장착하고 있는 상태라면 현재 무기를 장착해제할 수 있는지 체크한뒤 무기 장착 해제
		if(GetCurrentWeapon()->CanUnequip() == false)
			return;

		GetCurrentWeapon()->Unequip();
	}

	Weapons[(int32)InType]->Equip();

	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound() == true)
		OnWeaponTypeChanged.Broadcast(prevType, Type);
}

