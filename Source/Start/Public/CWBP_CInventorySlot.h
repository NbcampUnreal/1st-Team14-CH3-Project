#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IItemInterface.h" 
#include "CWBP_CInventorySlot.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class START_API UCWBP_CInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetItem(EItemType ItemType, int32 ItemCount);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSlotClicked();

    UFUNCTION()
    void OnSlotRightClicked();

    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountText;

    EItemType StoredItemType;
    int32 StoredItemCount;
};