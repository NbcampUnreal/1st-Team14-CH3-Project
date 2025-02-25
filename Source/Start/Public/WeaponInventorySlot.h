#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CWeaponInventoryComponent.h"
#include "WeaponInventorySlot.generated.h"

UCLASS()
class START_API UWeaponInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetWeaponSlotData(const FWeaponInventoryEntry& WeaponSlotData, int32 SlotIndex, UCWeaponInventoryComponent* InventoryRef);
    void ClearWeaponSlot();

private:
    UPROPERTY()
    UCWeaponInventoryComponent* WeaponInventoryComponent;

    UPROPERTY()
    int32 CurrentSlotIndex;

    UPROPERTY()
    FWeaponInventoryEntry CurrentWeaponSlot;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon;
};