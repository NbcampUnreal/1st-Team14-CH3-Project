#include "CWeapon_AK47.h"

#include "CCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "CMagazine.h"
#include "CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Components/SlateWrapperTypes.h"

ACWeapon_AK47::ACWeapon_AK47()
{
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
		LeftHandAimLocation = FVector(-32.453343,-1.028387,14.791615);
		WeapoLeftHandTransform = Mesh->GetSocketTransform("LeftHand");
	}

	//Aim
	{
		AimData.bEnableCameraLag = false;
		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 50, 10);
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
		MaxMagazineCount = 30;
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Reload/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
		if (montage.Succeeded() == true)
			ReloadMontage = montage.Object;
		ReloadPlayRate = 1.0f;
		MagazineBoneName = "b_gun_mag";
		static ConstructorHelpers::FClassFinder<ACMagazine> magazine(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_CMagazine.BP_CMagazine_C'"));
		if (magazine.Succeeded() == true)
			MagazineClass = magazine.Class;
		MagazinSocketName = "Rifle_Magazine";
	}

	// Arms
	{
		ArmsMeshTransform.SetLocation(FVector(-14.25f, -5.85f, -156.935f));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(-0.5f, -11.85f, -1.2f)));

		ArmsLeftHandTransform.SetLocation(FVector(-33, 11, -1.5f));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(-4, -138, 77)));
	}
}

void ACWeapon_AK47::BeginPlay()
{
	Super::BeginPlay();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if(player == nullptr)
		return;
	player->GetFirstPersonMesh()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_AK47::BeginAim()
{
	Super::BeginAim();
	//if(SightMesh->GetStaticMesh() == nullptr)
	//	return;
	////if (CrossHair != nullptr)
	////	CrossHair->SetVisibility(ESlateVisibility::Hidden);
	//ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//if(player == nullptr)
	//	return;
	//player->GetMesh()->SetVisibility(false);
	//player->GetFirstPersonMesh()->SetVisibility(true);

	//AttachToComponent(player->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandAimSokcetName);

	//UCWeaponComponent* weapon = Cast<UCWeaponComponent>(player->GetComponentByClass(UCWeaponComponent::StaticClass()));
	//if(weapon->OnWeaponAim_Arms_Begin.IsBound() == true)
	//	weapon->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_AK47::EndAim()
{
	Super::EndAim();
	//if (SightMesh->GetStaticMesh() == nullptr)
	//	return;
	////if (CrossHair != nullptr)
	////	CrossHair->SetVisibility(ESlateVisibility::Visible);
	//ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//if (player == nullptr)
	//	return;
	//player->GetFirstPersonMesh()->SetVisibility(false);
	//player->GetMesh()->SetVisibility(true);
	//AttachToComponent(player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSokcetName);

	//UCWeaponComponent* weapon = Cast<UCWeaponComponent>(player->GetComponentByClass(UCWeaponComponent::StaticClass()));
	//if (weapon->OnWeaponAim_Arms_End.IsBound() == true)
	//	weapon->OnWeaponAim_Arms_End.Broadcast();
}
