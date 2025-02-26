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

	// 무기가 해제되었으므로 HUD에 무기 이름을 비움
	OnWeaponNameChanged.Broadcast(FText::GetEmpty());

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

// ================
// 무기 타입 변경 함수
// ================
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

	// 무기 장착 시 바로 탄약 정보를 HUD에 전달
	int32 CurrentAmmo = Weapons[(int32)InType]->GetCurrentMagazineCount();
	int32 MaxAmmo = Weapons[(int32)InType]->GetMaxMagazineCount();
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo); // 즉시 업데이트

	// 무기 장착 시 무기 이름을 HUD에 전달
	if (ACWeapon* CurrentWeapon = GetCurrentWeapon())
	{
		FText WeaponName = CurrentWeapon->GetWeaponDisplayName();
		OnWeaponNameChanged.Broadcast(WeaponName);
	}
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

void UCWeaponComponent::Begin_Fire() // 무기 발사 시작
{
	if (GetCurrentWeapon() == nullptr)
		return;
	if (GetCurrentWeapon()->CanFire() == false)
		return;

	GetCurrentWeapon()->BeginFire();
	
	// 연발 시 매 발마다 HUD 업데이트를 위한 폴링 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(AmmoUpdateTimerHandle, this, &UCWeaponComponent::PollAmmoUpdate, 0.05f, true);
}

void UCWeaponComponent::End_Fire() // 무기 발사 종료
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->EndFire();

	GetWorld()->GetTimerManager().ClearTimer(AmmoUpdateTimerHandle); // 폴링 타이머 해제
}

void UCWeaponComponent::BeginAim()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->BeginAim();
	OnAimChanged.Broadcast(true); // Aim 상태 변경(줌인) 델리게이트 호출
}

void UCWeaponComponent::EndAim()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->EndAim();
	OnAimChanged.Broadcast(false); // Aim 상태 변경(줌아웃) 델리게이트 호출
}

bool UCWeaponComponent::GetInAim()
{
	if (GetCurrentWeapon() == nullptr)
		return false;

	return GetCurrentWeapon()->GetInAim();
}

FVector UCWeaponComponent::GetLefttHandLocation()
{
	if (GetCurrentWeapon() == nullptr)
		return FVector::Zero();

	return GetCurrentWeapon()->GetLeftHandLocation();
}

FVector UCWeaponComponent::GetLefttHandAimLocation()
{
	if (GetCurrentWeapon() == nullptr)
		return FVector::Zero();

	return GetCurrentWeapon()->GetLeftHandAimLocation();
}

void UCWeaponComponent::ToggleAutoFire()
{
	if(GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->ToggleAutoFire();
}

void UCWeaponComponent::Reload()
{
	if(GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->Reload();
}

void UCWeaponComponent::PollAmmoUpdate()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	// 연발 사격 중 매 발마다 탄약 정보를 HUD에 전달
	int32 CurrentAmmo = GetCurrentWeapon()->GetCurrentMagazineCount();
	int32 MaxAmmo = GetCurrentWeapon()->GetMaxMagazineCount();
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}

void UCWeaponComponent::Eject_Magazine()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->Eject_Magazine();
}

void UCWeaponComponent::Spawn_Magazine()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->Spawn_Magazine();
}

void UCWeaponComponent::Load_Magazine()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->Load_Magazine();
}

void UCWeaponComponent::End_Magazine()
{
	if (GetCurrentWeapon() == nullptr)
		return;

	GetCurrentWeapon()->End_Magazine();
	// 재장전 후 최신 탄약 정보 전달 (HUD 업데이트용)
	int32 CurrentAmmo = GetCurrentWeapon()->GetCurrentMagazineCount();
	int32 MaxAmmo = GetCurrentWeapon()->GetMaxMagazineCount();
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}

