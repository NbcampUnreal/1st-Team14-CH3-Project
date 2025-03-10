﻿#include "CWBP_CInventorySlot.h"
#include "CPlayer.h"
#include "CWBP_CInventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CInventoryComponent.h"
#include "GameFramework/HUD.h"
#include "CPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UCWBP_CInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UCWBP_CInventorySlot::OnSlotClicked);
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
        UE_LOG(LogTemp, Warning, TEXT("✅ 왼쪽 클릭 감지됨 - 아이템 사용 시도"));
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

void UCWBP_CInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

    if (!InventoryComponent || !InventoryComponent->GetOwner()) return;
    ACharacter* player = Cast<ACharacter>(InventoryComponent->GetOwner());
    if (player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ 플레이어를 찾을 수 없습니다!"));
		return;
	}
    // 🔹 플레이어 컨트롤러 가져오기
    APlayerController* PC = Cast<APlayerController>(player->GetController());
    if (!PC) return;

    ACPlayerController* CustomPC = Cast<ACPlayerController>(PC);
    if (!CustomPC) 
    {
		UE_LOG(LogTemp, Warning, TEXT("❌ 플레이어 컨트로러를 찾을 수 없습니다!"));
        return;
    }
    // 🔹 인벤토리 위젯 가져오기
    UCWBP_CInventory* InventoryWidget = CustomPC->GetInventoryWidget();
    if (!InventoryWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리 찾을 수 없습니다!"));
        return;
    }

    // 🔹 StoredItemType을 기반으로 아이템 객체 찾기
    IIItemInterface* FoundItem = InventoryComponent->FindItemByType(StoredItemType);
    if (!FoundItem) return;

    // ✅ 툴팁 업데이트 함수 호출 (IIItemInterface* 전달)
    InventoryWidget->UpdateItemTooltip(FoundItem);

    UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 툴팁 표시 - 타입: %d"), static_cast<int32>(StoredItemType));
}


void UCWBP_CInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (!InventoryComponent || !InventoryComponent->GetOwner()) return;

    // 🔹 플레이어 컨트롤러 가져오기
    APlayerController* PC = Cast<APlayerController>(InventoryComponent->GetOwner()->GetInstigatorController());
    if (!PC) return;

    ACPlayerController* CustomPC = Cast<ACPlayerController>(PC);
    if (!CustomPC) return;

    // 🔹 인벤토리 위젯 가져오기
    UCWBP_CInventory* InventoryWidget = CustomPC->GetInventoryWidget();
    if (!InventoryWidget) return;

    // ✅ 툴팁 숨기기 함수 호출
    InventoryWidget->HideItemTooltip();
}

void UCWBP_CInventorySlot::SetItem(EItemType ItemType, int32 ItemCount)
{
    StoredItemType = ItemType;
    StoredItemCount = ItemCount;

    if (ItemCountText)
    {
        ItemCountText->SetText(ItemCount > 0 ? FText::AsNumber(StoredItemCount) : FText::GetEmpty());
    }

    if (ItemImage)
    {
        if (ItemType == EItemType::EIT_None)
        {
            ItemImage->SetVisibility(ESlateVisibility::Hidden);  // ✅ 빈 슬롯이면 숨김
        }
        else
        {
            ItemImage->SetVisibility(ESlateVisibility::Visible);

            if (ItemBlueprintMap.Contains(ItemType))
            {
                TSubclassOf<ACBaseItem> BPClass = ItemBlueprintMap[ItemType];
                if (BPClass)
                {
                    ACBaseItem* DefaultObj = BPClass.GetDefaultObject();
                    if (DefaultObj && DefaultObj->ItemIcon)
                    {
                        ItemImage->SetBrushFromTexture(DefaultObj->ItemIcon);
                    }
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
    if (!InventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ InventoryComponent가 NULL이므로 아이템을 사용할 수 없습니다!"));
        return;
    }

    // 🔹 현재 플레이어 가져오기
    ACPlayer* Player = Cast<ACPlayer>(GetOwningPlayerPawn());
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 플레이어를 찾을 수 없습니다!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ OnSlotClicked 호출됨 - 아이템 사용 요청: %d"), static_cast<int32>(StoredItemType));

    // 🔹 무기인지 확인
    //if (StoredItemType == EItemType::EIT_Pistol)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("🔫 무기 장착 시도: %d"), static_cast<int32>(StoredItemType));

    //    InventoryComponent->EquipWeapon(StoredItemType, Player);
    //    return;
    //}

    // 🔹 일반 아이템 사용
    bool bUsed = InventoryComponent->UseItem(StoredItemType, Player);
    if (bUsed)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 사용됨: %d"), static_cast<int32>(StoredItemType));

        // 🔹 아이템이 삭제되었을 가능성이 있으므로, 인벤토리에 존재하는지 확인 후 UI 업데이트
        if (InventoryComponent->GetInventoryItems().Contains(StoredItemType))
        {
            InventoryComponent->OnInventoryUpdated.Broadcast();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ OnInventoryUpdated 호출 취소 - 아이템이 이미 삭제됨: %d"), static_cast<int32>(StoredItemType));
        }
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

    if (bIsDropping)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 이미 드랍 중인 아이템입니다."));
        return;
    }

    const TMap<EItemType, int32>& CurrentItems = InventoryComponent->GetInventoryItems();
    if (!CurrentItems.Contains(StoredItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 슬롯에 표시된 아이템(%d)은 인벤토리에 없습니다!"), static_cast<int32>(StoredItemType));
        return;
    }

    // ✅ RemoveItem 먼저 실행
    bool bRemoved = InventoryComponent->RemoveItem(StoredItemType);
    if (!bRemoved)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ RemoveItem 실패 - 인벤토리에서 제거되지 않음!"));
        return;
    }

    // ✅ 아이템 드랍 실행
    if (InventoryComponent->DropItem(StoredItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 정상 드랍됨: %d"), static_cast<int32>(StoredItemType));
        bIsDropping = true; // 드랍 성공 후에만 플래그 설정

        // 🚨 만약 드랍된 Bullet Box가 다시 추가되면 삭제
        if (StoredItemType == EItemType::EIT_BulletBox)
        {
            FTimerHandle RemoveTimer;
            GetWorld()->GetTimerManager().SetTimer(RemoveTimer, FTimerDelegate::CreateLambda([=, this]()
                {
                    if (!InventoryComponent)
                    {
                        UE_LOG(LogTemp, Error, TEXT("🚨 InventoryComponent가 nullptr임!"));
                        return;
                    }

                    if (InventoryComponent->GetInventoryItems().Contains(StoredItemType))
                    {
                        InventoryComponent->RemoveItem(StoredItemType);
                        UE_LOG(LogTemp, Error, TEXT("🚨 Bullet Box가 다시 추가됨 - 즉시 삭제!"));
                    }
                }), 0.1f, false);
        }

        InventoryComponent->OnInventoryUpdated.Broadcast();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 아이템 드랍 실패: %d"), static_cast<int32>(StoredItemType));
    }

    // ✅ 드랍 쿨다운 추가
    GetWorld()->GetTimerManager().SetTimer(DropCooldownTimerHandle, this, &UCWBP_CInventorySlot::ResetDropFlag, 0.1f, false);
}

