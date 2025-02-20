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

    if (ItemCountText)
    {
        ItemCountText->SetText(FText::AsNumber(StoredItemCount));
    }

    // 🔹 아이템 이미지 설정 (여기서는 임시로 설정, 블루프린트에서 아이콘 설정 가능)
    if (ItemImage)
    {
        // ItemImage->SetBrushFromTexture(YourItemTexture); // 블루프린트에서 설정 가능
    }
}

void UCWBP_CInventorySlot::OnSlotClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("아이템 사용: %d"), static_cast<int32>(StoredItemType));

    // 🔹 인벤토리에서 아이템 사용 로직 추가 가능
}

void UCWBP_CInventorySlot::OnSlotRightClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("아이템 버리기: %d"), static_cast<int32>(StoredItemType));

    // 🔹 아이템 버리기 로직 추가 가능
}
