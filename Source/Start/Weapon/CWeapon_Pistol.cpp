// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Pistol.h"

ACWeapon_Pistol::ACWeapon_Pistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/MilitaryWeapSilver/Weapons/Pistols_A.Pistols_A'"));
	if (mesh.Succeeded() == true)
		Mesh->SetSkeletalMesh(mesh.Object);

	//Equip
	{
		HolsterSocketName = NAME_None;
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimSequence'/Game/Assets/Animation/Military_Skel/Equip_Pistol_Standing.Equip_Pistol_Standing'"));
		if (montage.Succeeded() == true)
			EquipMontage = montage.Object;
		Equip_PlayRate = 1.0f;
		RightHandSokcetName = "Pistol_RightHand";
		LeftHandLocation = FVector(0,15,0);
	}
	//Aim
	{
		AimData.bEnableCameraLag = false;
		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 0, 10);
		AimData.FieldOfView = 55;
	}

	//Fire
	{

		FireRate = 1.0f;
		RecoilAngle = 0.75f;
		static ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_AK47_CameraShake.BP_AK47_CameraShake_C'"));
		if (cameraShake.Succeeded() == true)
			CameraShak = cameraShake.Class;
		AutoFireInterval = 0.15f;
		RecoilRate = 0.05f;
	}

	//Magazine
	{
		CurrentMagazineCount = 30;
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

}
