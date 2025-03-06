#include "CWeapon_Rifle.h"

#include "CCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "CMagazine.h"
#include "CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Components/SlateWrapperTypes.h"

ACWeapon_Rifle::ACWeapon_Rifle()
{
	ItemType = EItemType::EIT_Rifle;
	GunType = EGunType::AK;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (asset.Succeeded() == true)
		Mesh->SetSkeletalMesh(asset.Object);
	SightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sight Mesh"));
	SightMesh->SetupAttachment(Mesh,"Dotsight");
	SightMesh->SetCollisionProfileName("NoCollision");

	//Equip
	{
		HolsterSocketName = "Spine_Rifle";
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Equip/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
		if (montage.Succeeded() == true)
			EquipMontage = montage.Object;
		Equip_PlayRate = 1.0f;
		RightHandSokcetName = "Rifle_Hand";
		RightHandAimSokcetName = "Rifle_hand_Aim";
		LeftHandLocation = Mesh->GetSocketLocation("LeftHand");//FVector(-34.197836,12.642071,8.645998);
		LeftHandAimLocation = FVector(-29.665080, 5.216735, 8.753471);
		WeapoLeftHandTransform = Mesh->GetSocketTransform("LeftHand");
		static ConstructorHelpers::FObjectFinder<USoundWave> equipSound(TEXT("/Script/Engine.SoundWave'/Game/Sound/Gun_Equip.Gun_Equip'"));
		if (equipSound.Succeeded() == true)
			EquipSound = equipSound.Object;
		static ConstructorHelpers::FObjectFinder<USoundWave> unequipSound(TEXT("/Script/Engine.SoundWave'/Game/Sound/Gun_Unarmed.Gun_Unarmed'"));
		if (unequipSound.Succeeded() == true)
			UnequipSound = unequipSound.Object;
	}	

	//Aim
	{
		AimData.bEnableCameraLag = false;
		AimData.TargetArmLength = -50;
		//AimData.SocketOffset = FVector(-55, 50, 10);
		AimData.FieldOfView = 55;
	}

	//Fire
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Fire/Fire_Rifle_Hip_Montage.Fire_Rifle_Hip_Montage'"));
		if (montage.Succeeded() == true)
			FireMontage = montage.Object;
		FireRate = 1.0f;
		RecoilAngle = 0.75f;
		static ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_AK47_CameraShake.BP_AK47_CameraShake_C'"));
		if (cameraShake.Succeeded() == true)
			CameraShak = cameraShake.Class;
		static ConstructorHelpers::FClassFinder<UCameraShakeBase> aimCameraShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_AK47_CameraShake_Aim.BP_AK47_CameraShake_Aim_C'"));
		if (aimCameraShake.Succeeded() == true)
			AimCameraShak = aimCameraShake.Class;
		AutoFireInterval = 0.15f;
		RecoilRate = 0.05f;
	}
	
	//Magazine
	{
		ReloadMagazineCount = 30;
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Reload/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
		if (montage.Succeeded() == true)
			ReloadMontage = montage.Object;
		ReloadPlayRate = 1.0f;
		MagazineBoneName = "b_gun_mag";
		static ConstructorHelpers::FClassFinder<ACMagazine> magazine(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_CMagazine.BP_CMagazine_C'"));
		if (magazine.Succeeded() == true)
			MagazineClass = magazine.Class;
		MagazinSocketName = "Rifle_Magazine";
		static ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("/Script/Engine.SoundCue'/Game/Sound/GunReloadCue.GunReloadCue'"));
		if (sound.Succeeded() == true)
			ReloadSound= sound.Object;
	}

	// Arms
	{
		ArmsMeshTransform.SetLocation(FVector(34.881184, -11.844251, -152.998531));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(20.426988, 1.013832,-1.369104)));

		ArmsLeftHandTransform.SetLocation(FVector(-33, 11, -1.5f));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(-4, -138, 77)));
	}
}

void ACWeapon_Rifle::BeginPlay()
{
	Super::BeginPlay();
}

void ACWeapon_Rifle::BeginEquip()
{
	Super::BeginEquip();
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player == nullptr)
		return;
	//player->GetFirstPersonMesh()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_Rifle::BeginAim()
{
	Super::BeginAim();
	if(SightMesh->GetStaticMesh() == nullptr)
		return;
	
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if(player == nullptr)
		return;
	player->GetFirstPersonMesh()->SetVisibility(true);
	player->GetMesh()->SetVisibility(false);

	AttachToComponent(player->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandAimSokcetName);

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(player->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon->OnWeaponAim_Arms_Begin.IsBound() == true)
		weapon->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_Rifle::EndAim()
{
	Super::EndAim();
	if (SightMesh->GetStaticMesh() == nullptr)
		return;
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player == nullptr)
		return;
	player->GetFirstPersonMesh()->SetVisibility(false);
	player->GetMesh()->SetVisibility(true);
	AttachToComponent(player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSokcetName);
	
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(player->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon->OnWeaponAim_Arms_End.IsBound() == true)
		weapon->OnWeaponAim_Arms_End.Broadcast();
}
