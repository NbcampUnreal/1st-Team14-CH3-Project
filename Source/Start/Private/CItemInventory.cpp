// Fill out your copyright notice in the Description page of Project Settings.


#include "CItemInventory.h"
#include "CBaseItem.h"

CItemInventory::CItemInventory() :
	HealthPotionItemNum(0),
	StaminaPoitionItemNum(0),
	GrenadesItemNum(0)
{

}

CItemInventory::~CItemInventory()
{

}

void CItemInventory::PrintInventory()
{
	//CBaseItem** ItemInInventory = ItemInventory.GetData();
	//GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Green, FString::Printf(TEXT("%s"), ItemInventory[0]));
}

void CItemInventory::AddToInventory(IIItemInterface* ItemToAdd)
{
	GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Green, FString::Printf(TEXT("CItemInventory::AddToInventory")));
	switch (ItemToAdd->GetItemType())
	{
	case EItemType::EIT_HealthPotion:
		HealthPotionItemNum++;
		break;
	case EItemType::EIT_StaminaPotion:
		StaminaPoitionItemNum++;
		break;
	case EItemType::EIT_Bullet:
		break;
	case EItemType::EIT_Grenades:
		GrenadesItemNum++;
		break;
	default:
		break;
	}
}

void CItemInventory::UseHealthPotion(AActor* Target)
{

}

void CItemInventory::UseStaminaPotion(AActor* Target)
{

}

void CItemInventory::UseGrenades(AActor* Target)
{

}

