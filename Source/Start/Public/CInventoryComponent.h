// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

class ACBaseItem;
class IIItemInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class START_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

	//UFUNCTION(BlueprintCallable)
	//int GetHealthPotionItemNum();
	//UFUNCTION(BlueprintCallable)
	//int GetStaminaPotionItemNum();
	//UFUNCTION(BlueprintCallable)
	//int GetGrenadesNum();
	//UFUNCTION(BlueprintCallable)
	//void PrintInventory();

	UFUNCTION(BlueprintCallable)
	void AddToInventory(EItemType ItemType);

	UFUNCTION(BlueprintCallable)
	void UseHealthPotion();
	//UFUNCTION(BlueprintCallable)
	//void UseStaminaPotion();
	//UFUNCTION(BlueprintCallable)
	//void UseGrenades(AActor* Target);

protected:

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int HealthPotionItemNum;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int StaminaPoitionItemNum;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int GrenadesItemNum;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int BulletItemNum;
		
};
