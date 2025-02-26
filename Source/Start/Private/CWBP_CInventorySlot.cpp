#include "CWBP_CInventorySlot.h"
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


    // 인벤토리에서 해당 아이템이 있는지 확인
    ACPlayer* Player = Cast<ACPlayer>(GetOwningPlayerPawn());
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어를 찾을 수 없습니다!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ OnSlotClicked 호출됨 - 아이템 사용 요청: %d"), static_cast<int32>(StoredItemType));
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

