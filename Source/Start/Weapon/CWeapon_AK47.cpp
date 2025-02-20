#include "Weapon/CWeapon_AK47.h"

ACWeapon_AK47::ACWeapon_AK47()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (asset.Succeeded() == true)
		Mesh->SetSkeletalMesh(asset.Object);

}

void ACWeapon_AK47::BeginPlay()
{
	Super::BeginPlay();


}
