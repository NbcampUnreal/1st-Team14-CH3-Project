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
	UFUNCTION(BlueprintCallable)
	void UseStaminaPotion(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void UseGrenades(AActor* Target);

protected:

	UPROPERTY(VisibleAnywhere, Category="Item")
	int HealthPotionItemNum;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int StaminaPoitionItemNum;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int GrenadesItemNum;

	
};
