#include "CHealthPotionItem.h"

#include "SP_Pawn.h"
#include "CCharacter.h"

ACHealthPotionItem::ACHealthPotionItem()
{
	HealAmount = 20.f;
	ItemType = EItemType::EIT_HealthPotion;

}

void ACHealthPotionItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add HealthPotionItem To Inventory")));
}

void ACHealthPotionItem::Use(AActor* Target)
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use HealthPotionItem")));
	if (ACCharacter* CCharacter = Cast<ACCharacter>(Target))
	{		
		CCharacter->Heal(HealAmount);
		
	}
}
