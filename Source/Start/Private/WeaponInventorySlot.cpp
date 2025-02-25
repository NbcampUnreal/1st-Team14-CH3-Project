#include "WeaponInventorySlot.h"
#include "Components/Image.h"
#include "CWeaponInventoryComponent.h"

void UWeaponInventorySlot::SetWeaponSlotData(const FWeaponInventoryEntry& WeaponSlotData, int32 SlotIndex, UCWeaponInventoryComponent* InventoryRef)
{
    if (!InventoryRef) return;

    CurrentWeaponSlot = WeaponSlotData;
    CurrentSlotIndex = SlotIndex;
    WeaponInventoryComponent = InventoryRef;

    // ✅ 무기 아이콘 업데이트
    if (WeaponIcon && WeaponSlotData.WeaponIcon)
        WeaponIcon->SetBrushFromTexture(WeaponSlotData.WeaponIcon);
}

void UWeaponInventorySlot::ClearWeaponSlot()
{
    if (WeaponIcon)
    {
        WeaponIcon->SetBrushFromTexture(nullptr);  // 아이콘 제거
    }

    RemoveFromParent();  // ✅ UI 슬롯 자체 삭제
}
