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
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add HealthPotionItem To Inventory")));
}

void ACHealthPotionItem::Use(/*플레이어 클래스*/)
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use HealthPotionItem")));
	//if (Activator && Activator->ActorHasTag("Player"))
	//{
	//	if (ASP_Pawn* PlayerCharacter = Cast<ASP_Pawn>(Activator))
	//	{
	//		PlayerCharacter->AddHealth(HealAmount);
	//	}
	//}
}
