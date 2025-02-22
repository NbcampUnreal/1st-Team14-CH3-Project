#include "CWeapon_AK47.h"

#include "CPlayer.h"
#include "Components/CCameraComponent.h"

ACWeapon_AK47::ACWeapon_AK47()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (asset.Succeeded() == true)
		Mesh->SetSkeletalMesh(asset.Object);

	//Equip
	{
		HolsterSocketName = "Spine_Rifle";
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Equip_Rifle_Standing_Montage1.Equip_Rifle_Standing_Montage1'"));
		if (montage.Succeeded() == true)
			EquipMontage = montage.Object;
		Equip_PlayRate = 1.0f;
		RightHandSokcetName = "Rifle_Hand";
	}

	//Aim
	/*{
		BaseData.TargetArmLength = 300;
		BaseData.SocketOffset = FVector(0, 50, 15);
		BaseData.FieldOfView = 90;


		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 0, 10);
		AimData.FieldOfView = 55;
	}*/
}

void ACWeapon_AK47::BeginPlay()
{
	Super::BeginPlay();

}
