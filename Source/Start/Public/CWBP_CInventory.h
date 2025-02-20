// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWBP_CInventory.generated.h"

class UCInventoryComponent;
class UUniformGridPanel;
class UCWBP_CInventorySlot;

UCLASS()
class START_API UCWBP_CInventory : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateInventory(UCInventoryComponent* InventoryComponent);

protected:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* InventoryGrid;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCWBP_CInventorySlot> SlotWidgetClass;
};
