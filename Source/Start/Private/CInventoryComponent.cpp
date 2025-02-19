

#include "CInventoryComponent.h"
#include "CBaseItem.h"
#include "CPlayer.h"

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCInventoryComponent::AddToInventory(EItemType ItemType)
{
	GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Green, FString::Printf(TEXT("CItemInventory::AddToInventory")));
	switch (ItemType)
	{
	case EItemType::EIT_HealthPotion:
		HealthPotionItemNum++;
		break;
	case EItemType::EIT_StaminaPotion:
		StaminaPoitionItemNum++;
		break;
	case EItemType::EIT_Bullet:
		BulletItemNum++;
		break;
	case EItemType::EIT_Grenades:
		GrenadesItemNum++;
		break;
	default:
		break;
	}
}

void UCInventoryComponent::UseHealthPotion()
{
	if (HealthPotionItemNum > 0)
	{
		ACPlayer* CPlayer = Cast<ACPlayer>(GetOwner());
		if (CPlayer) {
			CPlayer->Heal(30);
		}
	}
}
