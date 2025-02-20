#include "CWBP_CInventory.h"
#include "CInventoryComponent.h"
#include "CWBP_CInventorySlot.h"
#include "Components/UniformGridPanel.h"

void UCWBP_CInventory::UpdateInventory(UCInventoryComponent* InventoryComponent)
{
    if (!InventoryComponent || !InventoryGrid) return;

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
    }
}
