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

    // 저장된 아이템 유형 및 개수
    EItemType StoredItemType;
    int32 StoredItemCount;

    // **아이템과 블루프린트 연결 (아이콘 포함)**
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemType, TSubclassOf<ACBaseItem>> ItemBlueprintMap;

    // **아이콘 가져오는 함수**
    UTexture2D* GetItemTexture(EItemType ItemType);

    // 슬롯 클릭 이벤트 (아이템 사용)
    UFUNCTION()
    void OnSlotClicked();

    // 슬롯 우클릭 이벤트 (아이템 버리기)
    UFUNCTION()
    void OnSlotRightClicked();
};
