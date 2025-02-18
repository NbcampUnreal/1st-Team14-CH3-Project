// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"

#include "SP_Pawn.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.f;
	ItemType = "Healing Item";
}

void AHealingItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("AddHealingItemToInventory")));
}

void AHealingItem::Use(/*플레이어 클래스*/)
{
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("Use Item")));
	//if (Activator && Activator->ActorHasTag("Player"))
	//{
	//	if (ASP_Pawn* PlayerCharacter = Cast<ASP_Pawn>(Activator))
	//	{
	//		PlayerCharacter->AddHealth(HealAmount);
	//	}
	//}
}
