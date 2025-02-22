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

    // ������ �߰�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddToInventory(EItemType ItemType);

    // ������ ����
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(EItemType ItemType);

    // ������ ��� (�ٴڿ� ������ ����)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool DropItem(EItemType ItemType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemAndUpdateUI(EItemType ItemType);

    // ���� �κ��丮 ���� ���
    UFUNCTION(BlueprintCallable)
    void PrintInventory();

    // �κ��丮 ���� �̺�Ʈ (UI���� ���ε��Ͽ� ���)
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UFUNCTION(BlueprintCallable)
    const TMap<EItemType, int32>& GetInventoryItems() const { return InventoryItems; }
protected:
    // �κ��丮 ���� ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int MaxSlots;

    // ���� �κ��丮�� �ִ� ������ (������ ���� ����)
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TMap<EItemType, int32> InventoryItems;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemType, TSubclassOf<ACBaseItem>> DropItemClasses;
};