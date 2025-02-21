#include "CWBP_CInventory.h"
#include "CInventoryComponent.h"
#include "CWBP_CInventorySlot.h"
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

    // 기존 슬롯 삭제
    InventoryGrid->ClearChildren();

    int32 SlotIndex = 0;
    TMap<EItemType, int32> Items = InventoryComponent->GetInventoryItems();
    UE_LOG(LogTemp, Warning, TEXT("Inventory에 등록된 아이템 수: %d"), Items.Num());

    // 인벤토리 컴포넌트에 등록된 모든 아이템에 대해 슬롯 생성
    for (const auto& Item : InventoryComponent->GetInventoryItems())
    {
        UCWBP_CInventorySlot* SlotWidget = CreateWidget<UCWBP_CInventorySlot>(this, SlotWidgetClass);
        if (SlotWidget)
        {
            SlotWidget->SetItem(Item.Key, Item.Value);
            // 여기서 InventoryComponent 전달
            SlotWidget->SetInventoryComponent(InventoryComponent);

            // 한 행에 5개씩 배치 (행: SlotIndex / 5, 열: SlotIndex % 5)
            InventoryGrid->AddChildToUniformGrid(SlotWidget, SlotIndex / 5, SlotIndex % 5);
            SlotIndex++;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 슬롯 위젯 생성 실패!"));
        }
    }
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