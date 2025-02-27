// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon_Knife.h"

ACWeapon_Knife::ACWeapon_Knife()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/MilitaryWeapSilver/Weapons/Knife_A.Knife_A'"));
	if (mesh.Succeeded() == true)
		Mesh->SetSkeletalMesh(mesh.Object);
}
