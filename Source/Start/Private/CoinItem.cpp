// Fill out your copyright notice in the Description page of Project Settings.
#include "CoinItem.h"
#include "Engine/World.h"
#include "MyGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoinItem";
}

void ACoinItem::KeyPressedActivate(AActor* Activator)
{

}

void ACoinItem::Use()
{
	//if (Activator && Activator->ActorHasTag("Player"))
	//{
	//	if (UWorld* World = GetWorld())
	//	{
	//		if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
	//		{
	//			GameState->AddScore(PointValue);
	//			GameState->OnCoinCollected();
	//		}
	//	}
	//}
}
