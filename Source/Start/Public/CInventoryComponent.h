#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IItemInterface.h"
#include "Delegates/DelegateCombinations.h" 
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class ACBaseItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCInventoryComponent();

    // 아이템 추가
    UFUNCTION(BlueprintCallable)
    bool AddToInventory(EItemType ItemType);

    // 아이템 제거
    UFUNCTION(BlueprintCallable)
    bool RemoveItem(EItemType ItemType);

    // 아이템 드롭 (바닥에 아이템 생성)
    UFUNCTION(BlueprintCallable)
    bool DropItem(EItemType ItemType);

    // 현재 인벤토리 상태 출력
    UFUNCTION(BlueprintCallable)
    void PrintInventory();

    // 인벤토리 변경 이벤트 (UI에서 바인딩하여 사용)
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UFUNCTION(BlueprintCallable)
    const TMap<EItemType, int32>& GetInventoryItems() const { return InventoryItems; }
protected:
    // 인벤토리 슬롯 개수 제한
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int MaxSlots;

    // 현재 인벤토리에 있는 아이템 (종류별 개수 저장)
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TMap<EItemType, int32> InventoryItems;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemType, TSubclassOf<ACBaseItem>> DropItemClasses;
};