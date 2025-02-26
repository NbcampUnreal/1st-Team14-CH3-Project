#include "Weapon/CWeapon_AR4.h"

ACWeapon_AR4::ACWeapon_AR4()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (asset.Succeeded() == true)
		Mesh->SetSkeletalMesh(asset.Object);

	//Equip
	{
		HolsterSocketName = "Spine_Rifle";
		static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Montages/Rifle/Equip/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
		if (montage.Succeeded() == true)
			EquipMontage = montage.Object;
		Equip_PlayRate = 1.0f;
		RightHandSokcetName = "Rifle_Hand";
		LeftHandLocation = FVector(-34.197836, 12.642071, 8.645998);
		LeftHandAimLocation = FVector(-32.453343, -1.028387, 14.791615);
	}
}
