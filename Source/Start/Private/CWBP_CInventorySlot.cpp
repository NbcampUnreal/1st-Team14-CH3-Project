﻿#include "CWBP_CInventorySlot.h"
#include "CPlayer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CInventoryComponent.h"


void UCWBP_CInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
    }
}

FReply UCWBP_CInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ InventoryComponent가 없음!"));
        return FReply::Unhandled();
    }

    // 🔹 왼쪽 클릭 감지
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ 왼쪽 클릭 감지됨 - 아이템 사용"));
        OnSlotClicked();
        return FReply::Handled();
    }

    // 🔹 오른쪽 클릭 감지
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ 오른쪽 클릭 감지됨 - 아이템 버리기"));
        OnSlotRightClicked();
        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
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
                    return;
                }
            }
        }
        UE_LOG(LogTemp, Error, TEXT("❌ 블루프린트에서 아이콘을 가져올 수 없음!"));
    }
}

void UCWBP_CInventorySlot::SetInventoryComponent(UCInventoryComponent* InInventoryComponent)
{
    InventoryComponent = InInventoryComponent;
}

void UCWBP_CInventorySlot::OnSlotClicked()
{
    if (!InventoryComponent) return;

    // 인벤토리에서 해당 아이템이 있는지 확인
    ACPlayer* Player = Cast<ACPlayer>(GetOwningPlayerPawn());
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어를 찾을 수 없습니다!"));
        return;
    }

    // 아이템 사용
    bool bUsed = InventoryComponent->UseItem(StoredItemType, Player);
    if (bUsed)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 사용됨: %d"), static_cast<int32>(StoredItemType));

        // UI 업데이트 강제 실행
        InventoryComponent->OnInventoryUpdated.Broadcast();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 아이템 사용 실패: %d"), static_cast<int32>(StoredItemType));
    }
}

void UCWBP_CInventorySlot::ResetDropFlag()
{
    bIsDropping = false;
}

void UCWBP_CInventorySlot::OnSlotRightClicked()
{
        if (!InventoryComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("❌ OnSlotRightClicked 실행 취소 - InventoryComponent가 없음!"));
            return;
        }

        // 🔹 인벤토리 상태 확인 (디버그 로그 추가)
        const TMap<EItemType, int32>& CurrentItems = InventoryComponent->GetInventoryItems();
        UE_LOG(LogTemp, Warning, TEXT("📌 현재 인벤토리 상태:"));
        for (const auto& Item : CurrentItems)
        {
            UE_LOG(LogTemp, Warning, TEXT("- 아이템: %d, 개수: %d"), static_cast<int32>(Item.Key), Item.Value);
        }

        if (InventoryComponent->DropItem(StoredItemType))
        {
            UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 정상 드랍됨: %d"), static_cast<int32>(StoredItemType));

            // ✅ UI 업데이트 강제 실행
            InventoryComponent->OnInventoryUpdated.Broadcast();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("❌ 아이템 드랍 실패: %d"), static_cast<int32>(StoredItemType));
        }

    if (CurrentItems.Contains(StoredItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem 호출됨: %d"), static_cast<int32>(StoredItemType));

        // ✅ 중복 실행 방지
        if (bIsDropping)
        {
            UE_LOG(LogTemp, Warning, TEXT("❌ 이미 드랍 중인 아이템입니다."));
            return;
        }
        bIsDropping = true;

        bool bRemoved = InventoryComponent->RemoveItem(StoredItemType);
        if (!bRemoved)
        {
            UE_LOG(LogTemp, Warning, TEXT("슬롯에 표시된 아이템이 실제 인벤토리에 존재하지 않습니다!"));
        }

        // ✅ 0.1초 후 다시 클릭 가능
        GetWorld()->GetTimerManager().SetTimer(DropCooldownTimerHandle, this, &UCWBP_CInventorySlot::ResetDropFlag, 0.1f, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("슬롯에 표시된 아이템(%d)은 인벤토리에 없습니다!"), static_cast<int32>(StoredItemType));
    }
}

