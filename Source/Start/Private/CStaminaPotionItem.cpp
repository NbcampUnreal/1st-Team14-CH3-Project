// Fill out your copyright notice in the Description page of Project Settings.


#include "CStaminaPotionItem.h"

ACStaminaPotionItem::ACStaminaPotionItem()
{
}

void ACStaminaPotionItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add StaminaPotionItem To Inventory")));
}

void ACStaminaPotionItem::Use()
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use StaminaPotionItem")));
}
