#include "CWBP_CInventory.h"
#include "CInventoryComponent.h"
#include "CWBP_CInventorySlot.h"
#include "Components/UniformGridPanel.h"

void UCWBP_CInventory::UpdateInventory(UCInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventoryGrid)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ InventoryComponent 또는 InventoryGrid가 NULL입니다!"));
        return;
    }

    if (!SlotWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SlotWidgetClass가 NULL입니다! 블루프린트에서 확인 필요!"));
        return;
    }

    InventoryGrid->ClearChildren();

    int32 SlotIndex = 0;
    for (const auto& Item : InventoryComponent->GetInventoryItems())
    {
        UCWBP_CInventorySlot* SlotWidget = CreateWidget<UCWBP_CInventorySlot>(this, SlotWidgetClass);
        if (SlotWidget)
        {
            SlotWidget->SetItem(Item.Key, Item.Value);
            InventoryGrid->AddChildToUniformGrid(SlotWidget, SlotIndex / 5, SlotIndex % 5);
            SlotIndex++;
        }
        if (!SlotWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 슬롯 위젯 생성 실패!"));
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("🔹 %d개의 슬롯이 추가되었습니다."), SlotIndex);
}
