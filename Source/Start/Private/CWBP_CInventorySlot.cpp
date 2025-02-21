#include "CWBP_CInventorySlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CInventoryComponent.h"


void UCWBP_CInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UCWBP_CInventorySlot::OnSlotClicked);
        SlotButton->OnReleased.AddDynamic(this, &UCWBP_CInventorySlot::OnSlotRightClicked);
    }
}

void UCWBP_CInventorySlot::SetItem(EItemType ItemType, int32 ItemCount)
{
    StoredItemType = ItemType;
    StoredItemCount = ItemCount;

    UE_LOG(LogTemp, Warning, TEXT("✅ 슬롯에 아이템 추가: %d (수량: %d)"), (int32)ItemType, ItemCount);

    if (ItemCountText)
    {
        ItemCountText->SetText(FText::AsNumber(StoredItemCount));
    }

    // 블루프린트에서 아이콘 가져오기 (디버그 로그 추가)
    if (ItemImage)
    {
        if (ItemBlueprintMap.Contains(ItemType))
        {
            TSubclassOf<ACBaseItem> BPClass = ItemBlueprintMap[ItemType];
            if (BPClass)
            {
                ACBaseItem* DefaultObj = BPClass.GetDefaultObject();
                if (DefaultObj && DefaultObj->ItemIcon)
                {
                    UE_LOG(LogTemp, Warning, TEXT("▶ 아이콘 텍스처: %s"), *DefaultObj->ItemIcon->GetName());
                    ItemImage->SetBrushFromTexture(DefaultObj->ItemIcon);
                }
            }
        }
    }
}

void UCWBP_CInventorySlot::SetInventoryComponent(UCInventoryComponent* InInventoryComponent)
{
    InventoryComponent = InInventoryComponent;
}

void UCWBP_CInventorySlot::OnSlotClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("아이템 버리기: %d"), static_cast<int32>(StoredItemType));

    // 만약 InventoryComponent 포인터가 있다면 실제 인벤토리 상태 재확인
    if (InventoryComponent)
    {
        // 인벤토리에 이 아이템이 존재하는지 확인
        const TMap<EItemType, int32>& CurrentItems = InventoryComponent->GetInventoryItems();
        if (CurrentItems.Contains(StoredItemType))
        {
            UE_LOG(LogTemp, Warning, TEXT("RemoveItem 호출됨: %d"), static_cast<int32>(StoredItemType));
            bool bRemoved = InventoryComponent->RemoveItem(StoredItemType);
            if (!bRemoved)
            {
                UE_LOG(LogTemp, Warning, TEXT("슬롯에 표시된 아이템이 실제 인벤토리에 존재하지 않습니다!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("슬롯에 표시된 아이템(%d)은 인벤토리에 없습니다!"), static_cast<int32>(StoredItemType));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryComponent가 슬롯에 할당되어 있지 않습니다!"));
    }
}

void UCWBP_CInventorySlot::OnSlotRightClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("아이템 버리기: %d"), static_cast<int32>(StoredItemType));

    // 🔹 아이템 버리기 로직 추가 가능
}

