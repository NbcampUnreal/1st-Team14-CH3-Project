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
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Pistol/Equip_Pistol_Standing_Montage.Equip_Pistol_Standing_Montage'"));
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

	////Fire
	//{
	//	FireRate = 1.0f;
	//	RecoilAngle = 0.75f;
	//	static ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/AK/BP_AK47_CameraShake.BP_AK47_CameraShake_C'"));
	//	if (cameraShake.Succeeded() == true)
	//		CameraShak = cameraShake.Class;
	//	AutoFireInterval = 6.0f;
	//	RecoilRate = 4.0f;
	//}

	////Magazine
	//{
	//	MaxMagazineCount = 5;
	//	static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Pistol/Reload_Pistol_Montage.Reload_Pistol_Montage'"));
	//	if (montage.Succeeded() == true)
	//		ReloadMontage = montage.Object;
	//	ReloadPlayRate = 1.5f;
	//	MagazineBoneName = NAME_None;
	//	static ConstructorHelpers::FClassFinder<ACMagazine> magazine(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/Pistol/BP_CMagazine_Pistol.BP_CMagazine_Pistol_C'"));
	//	if (magazine.Succeeded() == true)
	//		MagazineClass = magazine.Class;
	//	MagazinSocketName = "Pistol_Magazine";
	//}

}

void ACWeapon_Pistol::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetVisibility(false);
}

void ACWeapon_Pistol::EndEquip()
{
	Super::EndEquip();
}

void ACWeapon_Pistol::BeginEquip()
{
	Super::BeginEquip();
	
	Mesh->SetVisibility(true);
}

void ACWeapon_Pistol::BeginAim()
{
	Super::BeginAim();
}

void ACWeapon_Pistol::EndAim()
{
	Super::EndAim();
}
