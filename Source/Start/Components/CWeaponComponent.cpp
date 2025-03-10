#include "Components/CWeaponComponent.h"
#include "CCharacter.h"
#include "Weapon/CWeapon.h"
#include "Weapon/CWeapon_Rifle.h"

bool UCWeaponComponent::IsRifleMode()
{
	return Type == EWeaponType::Rifle;
}

bool UCWeaponComponent::IsPistolMode()
{
	return Type == EWeaponType::Pistol;
}
UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

	OnWeaponAim_Arms_Begin.AddDynamic(this, &UCWeaponComponent::On_Begin_Aim);
	OnWeaponAim_Arms_End.AddDynamic(this, &UCWeaponComponent::On_End_Aim);
}

void UCWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(IsRifleMode() == true || IsPistolMode() == true)
		OnAmmoChanged.Broadcast(GetCurrentWeapon()->GetCurrentMagazineCount(),GetCurrentWeapon()->GetMaxMagazineCount() );
}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{
	if (IsUnarmedMode() == true)
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
	int32 count{};
	for(auto a : Weapons)
	{
		if (a->GetItemType() == EItemType::EIT_Rifle)
			count++;
	}
	if(count == 0)
		return;
	SetMode(EWeaponType::Rifle);
}

void UCWeaponComponent::SetKnifeMode()
{
	int32 count{};
	for (auto a : Weapons)
	{
		if (a->GetItemType() == EItemType::EIT_Knife)
			count++;
	}
	if (count == 0)
		return;

	SetMode(EWeaponType::Knife);
}

void UCWeaponComponent::SetGrenadeMode()
{
	/*int32 count{};
	for (auto a : Weapons)
	{
		if (a->GetItemType() == EItemType::EIT_Grenades)
			count++;
	}
	if (count == 0)
		return;*/

	SetMode(EWeaponType::Grenade);
}

void UCWeaponComponent::SetPistolMode()
{
	int32 count{};
	for (auto a : Weapons)
	{
		if (a->GetItemType() == EItemType::EIT_Pistol)
			count++;
	}
	if (count == 0)
		return;
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
	else if(IsUnarmedMode() == false)
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
	//int32 CurrentAmmo = Weapons[(int32)InType]->GetCurrentMagazineCount();
	//int32 MaxAmmo = Weapons[(int32)InType]->GetMaxMagazineCount();
	//OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo); // 즉시 업데이트

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

void UCWeaponComponent::DoAction()
{if(GetCurrentWeapon() == nullptr)
		return;
	if (IsRifleMode() == true || IsPistolMode() == true)
	{
		Begin_Fire();
		return;
	}

	GetCurrentWeapon()->DonAction();
}

void UCWeaponComponent::Begin_DoAction()
{
	if(GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->BeginAction();
}

void UCWeaponComponent::End_DoAction()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	GetCurrentWeapon()->EndAction();
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

bool UCWeaponComponent::BeginAim()
{
	if (GetCurrentWeapon() == nullptr)
		return false;
	if(GetCurrentWeapon()->CanAim() == false)
		return false;

	GetCurrentWeapon()->BeginAim();
	OnAimChanged.Broadcast(true); // Aim 상태 변경(줌인) 델리게이트 호출
	return true;
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

FVector UCWeaponComponent::GetLeftHandLocation()
{
	if (GetCurrentWeapon() == nullptr)
		return FVector::Zero();

	return GetCurrentWeapon()->GetLeftHandLocation();
}

FVector UCWeaponComponent::GetLeftHandAimLocation()
{
	if (GetCurrentWeapon() == nullptr)
		return FVector::Zero();

	return GetCurrentWeapon()->GetLeftHandAimLocation();
}

FTransform UCWeaponComponent::GetLeftArmsLeftHandTransform()
{
	if (GetCurrentWeapon() == nullptr)
		return FTransform();

	return GetCurrentWeapon()->GetLeftArmsLeftHandTransform();
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
	if(GetCurrentWeapon()->CanReload() == false)
		return;
	GetCurrentWeapon()->Reload();
}

void UCWeaponComponent::PollAmmoUpdate()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	// 연발 사격 중 매 발마다 탄약 정보를 HUD에 전달
	/*int32 CurrentAmmo = GetCurrentWeapon()->GetCurrentMagazineCount();
	int32 MaxAmmo = GetCurrentWeapon()->GetMaxMagazineCount();
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);*/
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
	/*int32 CurrentAmmo = GetCurrentWeapon()->GetCurrentMagazineCount();
	int32 MaxAmmo = GetCurrentWeapon()->GetMaxMagazineCount();
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);*/
}

void UCWeaponComponent::On_Begin_Aim(ACWeapon* InThisWeapon)
{
	if(GetCurrentWeapon() == nullptr)
		return;
	for (ACWeapon* weapon : Weapons)
	{
		if (weapon != InThisWeapon)
			weapon->SetHidden(true);
		else
			weapon->SetHidden(false);
	}
}

void UCWeaponComponent::On_End_Aim()
{
	if (GetCurrentWeapon() == nullptr)
		return;
	for (ACWeapon* weapon : Weapons)
			weapon->SetHidden(false);
}

int32 UCWeaponComponent::GetWeaponIndexFromItemType(EItemType ItemType)
{
	
	/*if(GetCurrentWeapon() != nullptr && GetCurrentWeapon()->GetItemType() == ItemType)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 같은 타입의 무기 장착중입니다."));
		return -1;
	}

	for (auto a : WeaponClasses)
	{
		ACWeapon* weapon = NewObject<ACWeapon>(a);
		if (weapon->GetIteypType() == ItemType)
		{
			weapon->ConditionalBeginDestroy();
			UE_LOG(LogTemp, Error, TEXT("❌ 같은 타입의 무기 장착중입니다."));
			return -1;
		}
		weapon->ConditionalBeginDestroy();
	}*/

	switch (ItemType)
	{
	case EItemType::EIT_Rifle:
		TSubclassOf<ACWeapon_Rifle> rifle = ACWeapon_Rifle::StaticClass();
		WeaponClasses.Add(rifle);
		if (rifle == nullptr)
			UE_LOG(LogTemp, Warning, L"1");
		FActorSpawnParameters param;
		param.Owner = Owner;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ACWeapon* W = Owner->GetWorld()->SpawnActor<ACWeapon_Rifle>(rifle, param);
		Weapons.Add(W);
		break;
		
	/*case EItemType::EIT_Pistol:
		TSubclassOf<ACWeapon> pistol = ACWeapon::StaticClass();
		WeaponClasses.Add(pistol);
		break;*/
	}

	/*switch (ItemType)
	{
	case EItemType::EIT_Pistol: return 0;
	case EItemType::EIT_Rifle: return 1;
	case EItemType::EIT_Shotgun: return 2;
	default:
		UE_LOG(LogTemp, Error, TEXT("❌ GetWeaponIndexFromItemType: 잘못된 ItemType (%d)"), (int32)ItemType);
		return -1;
	}*/


	return 0;
}