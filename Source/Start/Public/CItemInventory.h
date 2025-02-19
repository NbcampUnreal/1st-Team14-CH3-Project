// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ACBaseItem;
class IIItemInterface;

class START_API CItemInventory
{
public:
	CItemInventory();
	~CItemInventory();

	UFUNCTION(BlueprintCallable)
	int GetHealthPotionItemNum();
	UFUNCTION(BlueprintCallable)
	int GetStaminaPotionItemNum();
	UFUNCTION(BlueprintCallable)
	int GetGrenadesNum();
	UFUNCTION(BlueprintCallable)
	void PrintInventory();

	UFUNCTION(BlueprintCallable)
	void AddToInventory(IIItemInterface* Item);

	UFUNCTION(BlueprintCallable)
	void UseHealthPotion(AActor* Target);
	void UseStaminaPotion(AActor* Target);
	void UseGrenades(AActor* Target);
protected:
	class CHealthPotionItem* HealthPotion;
	class CStaminaPotionItem* StaminaPotion;
	class CGrenadesItem* Grenades;

	UPROPERTY(VisibleAnywhere)
	int HealthPotionItemNum;
	UPROPERTY(VisibleAnywhere)
	int StaminaPoitionItemNum;
	UPROPERTY(VisibleAnywhere)
	int GrenadesItemNum;

	
};
