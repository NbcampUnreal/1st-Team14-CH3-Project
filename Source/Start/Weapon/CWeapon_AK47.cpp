#include "CWeapon_AK47.h"
#include "CPlayer.h"
#include "Components/CCameraComponent.h"
#include "Camera/CameraShakeBase.h"

ACWeapon_AK47::ACWeapon_AK47()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (asset.Succeeded() == true)
		Mesh->SetSkeletalMesh(asset.Object);

	//Equip
	{
		HolsterSocketName = "Spine_Rifle";
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Equip/Equip_Rifle_Standing_Montage1.Equip_Rifle_Standing_Montage1'"));
		if (montage.Succeeded() == true)
			EquipMontage = montage.Object;
		Equip_PlayRate = 1.0f;
		RightHandSokcetName = "Rifle_Hand";
		LeftHandLocation = FVector(-37.119789,14.315624,3.738997);//Mesh->GetSocketLocation("LeftHand");
	}
	

	//Aim
	/*{
		BaseData.bEnableCameraLag = true;
		BaseData.TargetArmLength = 300;
		BaseData.SocketOffset = FVector(0, 50, 15);
		BaseData.FieldOfView = 90;


		AimData.bEnableCameraLag = true;
		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 0, 10);
		AimData.FieldOfView = 55;
	}*/

	//Fire
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Fire/Fire_Rifle_Hip_Montage.Fire_Rifle_Hip_Montage'"));
		if (montage.Succeeded() == true)
			FireMontage = montage.Object;
		FireRate = 1.0f;
		RecoilAngle = 0.75f;
		static ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/BP_AK47_CameraShake.BP_AK47_CameraShake_C'"));
		if (cameraShake.Succeeded() == true)
			CameraShak = cameraShake.Class;
		AutoFireInterval = 0.15f;
		RecoilRate = 0.05f;
	}
}

void ACWeapon_AK47::BeginPlay()
{
	Super::BeginPlay();
}