#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IItemInterface.h" 
#include "CBaseItem.h"
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
    virtual void NativeConstruct() override;
protected:
    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountText;

    // ����� ������ ���� �� ����
    EItemType StoredItemType;
    int32 StoredItemCount;

    // **�����۰� �������Ʈ ���� (������ ����)**
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemType, TSubclassOf<ACBaseItem>> ItemBlueprintMap;

    // **������ �������� �Լ�**
    UTexture2D* GetItemTexture(EItemType ItemType);

    // ���� Ŭ�� �̺�Ʈ (������ ���)
    UFUNCTION()
    void OnSlotClicked();

    // ���� ��Ŭ�� �̺�Ʈ (������ ������)
    UFUNCTION()
    void OnSlotRightClicked();
};
