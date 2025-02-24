#include "CWBP_CInventory.h"
#include "CInventoryComponent.h"
#include "CWBP_CInventorySlot.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UCWBP_CInventory::InitializeInventory(UCInventoryComponent* InInventoryComponent)
{
    if (!InInventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ InInventoryComponent가 NULL입니다!"));
        return;
    }

    InventoryComponent = InInventoryComponent;
    UE_LOG(LogTemp, Warning, TEXT("✅ InventoryComponent가 초기화되었습니다."));

    // delegate를 바인딩 (이미 바인딩되어 있지 않은지 확인)
    if (!bDelegateBound)
    {
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UCWBP_CInventory::UpdateInventory);
        bDelegateBound = true;
    }

    UpdateInventory();
}

void UCWBP_CInventory::UpdateInventory()
{
    UE_LOG(LogTemp, Warning, TEXT(">>> UpdateInventory 함수 호출됨."));

    if (!InventoryComponent || !InventoryWrapBox)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ InventoryComponent 또는 InventoryWrapBox가 NULL입니다!"));
        return;
    }

    if (!SlotWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SlotWidgetClass가 NULL입니다! 블루프린트에서 확인 필요!"));
        return;
    }

    // 기존 슬롯 삭제
    InventoryWrapBox->ClearChildren();

    // ✅ 정렬 순서를 보장하기 위해 TArray로 변환
    TArray<EItemType> SortedItems;
    InventoryComponent->GetInventoryItems().GenerateKeyArray(SortedItems);

    // ✅ 아이템을 저장된 순서대로 정렬 (필요시 사용자 지정 순서 적용 가능)
    SortedItems.Sort([](const EItemType& A, const EItemType& B)
        {
            return static_cast<int32>(A) < static_cast<int32>(B);
        });

    // ✅ 정렬된 순서대로 슬롯 추가
    int32 SlotIndex = 0;
    for (const EItemType& ItemType : SortedItems)
    {
        int32 ItemCount = InventoryComponent->GetInventoryItems()[ItemType];

        UCWBP_CInventorySlot* SlotWidget = CreateWidget<UCWBP_CInventorySlot>(this, SlotWidgetClass);
        if (SlotWidget)
        {
            SlotWidget->SetItem(ItemType, ItemCount);
            SlotWidget->SetInventoryComponent(InventoryComponent);

            // 🔹 WrapBox에 추가하여 자동 정렬 유지
            InventoryWrapBox->AddChild(SlotWidget);

            SlotIndex++;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 슬롯 위젯 생성 실패!"));
        }
    }
}

void UCWBP_CInventory::UpdateItemTooltip(EItemType ItemType)
{
    if (!T_ItemName || !T_ItemDescription) return;

    // ✅ `EItemType`을 문자열로 변환 후, UI에 표시할 이름 설정
    FString ItemName = UEnum::GetDisplayValueAsText(ItemType).ToString();
    FString ItemDescription = TEXT("이 아이템은 특정 효과가 있습니다."); // 설명 추가 가능

    // ✅ UI 업데이트
    T_ItemName->SetText(FText::FromString(ItemName));
    T_ItemDescription->SetText(FText::FromString(ItemDescription));

    // ✅ 툴팁이 보이도록 강제 활성화
    T_ItemName->SetVisibility(ESlateVisibility::Visible);
    T_ItemDescription->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Warning, TEXT("📌 아이템 툴팁 업데이트 - 이름: %s"), *ItemName);
}

void UCWBP_CInventory::HideItemTooltip()
{
    if (!T_ItemName || !T_ItemDescription) return;

    // ✅ UI 숨기기
    T_ItemName->SetVisibility(ESlateVisibility::Hidden);
    T_ItemDescription->SetVisibility(ESlateVisibility::Hidden);

    UE_LOG(LogTemp, Warning, TEXT("📌 아이템 툴팁 숨김"));
}


void UCWBP_CInventory::NativeDestruct()
{
    // 위젯이 파괴되기 전에 델리게이트 연결 해제
    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &UCWBP_CInventory::UpdateInventory);
        bDelegateBound = false;
        UE_LOG(LogTemp, Warning, TEXT("🛑 InventoryComponent와의 델리게이트 연결 해제됨."));
    }
    Super::NativeDestruct();
}
