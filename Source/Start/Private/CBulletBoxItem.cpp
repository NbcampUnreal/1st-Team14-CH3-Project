// Fill out your copyright notice in the Description page of Project Settings.


#include "CBulletBoxItem.h"

ACBulletBoxItem::ACBulletBoxItem()
{
	ItemType = EItemType::EIT_Bullet;
}

void ACBulletBoxItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add BulletBoxItem To Inventory")));
}
