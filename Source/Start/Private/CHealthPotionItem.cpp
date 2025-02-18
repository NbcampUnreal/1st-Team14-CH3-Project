// Fill out your copyright notice in the Description page of Project Settings.


#include "CHealthPotionItem.h"

#include "SP_Pawn.h"

ACHealthPotionItem::ACHealthPotionItem()
{
	HealAmount = 20.f;
	ItemType = "Healing Item";
}

void ACHealthPotionItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("AddHealingItemToInventory")));
}

void ACHealthPotionItem::Use(/*�÷��̾� Ŭ����*/)
{
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("Use HealItem")));
	//if (Activator && Activator->ActorHasTag("Player"))
	//{
	//	if (ASP_Pawn* PlayerCharacter = Cast<ASP_Pawn>(Activator))
	//	{
	//		PlayerCharacter->AddHealth(HealAmount);
	//	}
	//}
}
