// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"

#include "SP_Pawn.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.f;
	ItemType = "Healing Item";
}

void AHealingItem::OverlapActivate(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASP_Pawn* PlayerCharacter = Cast<ASP_Pawn>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
