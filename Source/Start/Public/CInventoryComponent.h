﻿#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IItemInterface.h"
#include "Delegates/DelegateCombinations.h" 
#include "Components/CWeaponComponent.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class ACBaseItem;
class ACPlayer;

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class START_API UCInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UCWeaponComponent* WeaponComponent;  // 무기 관리 컴포넌트

    // 🔹 무기 장착
    //void EquipWeapon(EItemType WeaponType, ACPlayer* Player);

    UCInventoryComponent();
    void BeginPlay();

    FORCEINLINE int32 CurrentBulletCount() { return InventoryItems[EItemType::EIT_Bullet]; }

    IIItemInterface* FindItemByType(EItemType ItemType);
    // 아이템 추가
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddToInventory(EItemType ItemType);

    // 아이템 제거
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(EItemType ItemType);

    // 아이템 드롭 (바닥에 아이템 생성)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool DropItem(EItemType ItemType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemAndUpdateUI(EItemType ItemType);
    
    // ✅ 아이템 사용 추가
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UseItem(EItemType ItemType, ACPlayer* Player);

    // 현재 인벤토리 상태 출력
    UFUNCTION(BlueprintCallable)
    void PrintInventory();

    // 인벤토리 변경 이벤트 (UI에서 바인딩하여 사용)
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UFUNCTION(BlueprintCallable)
    const TMap<EItemType, int32>& GetInventoryItems() const { return InventoryItems; }

    void AddBulletsToInventory(int32 BulletCount);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetBulletCount() const;
    int32 UseBulletCount(int32 Amount);
protected:
    // 인벤토리 슬롯 개수 제한
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int MaxSlots;

    // 현재 인벤토리에 있는 아이템 (종류별 개수 저장)
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TMap<EItemType, int32> InventoryItems;

    // 📌 아이템 상세 정보 저장 (새로 추가)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TMap<EItemType, FItemData> ItemDetails;

    // 📌 아이템 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UDataTable* ItemDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TMap<EItemType, TSubclassOf<ACBaseItem>> DropItemClasses;

private:
    // ✅ 아이템 인스턴스를 가져오는 함수 추가
    ACBaseItem* GetItemInstance(EItemType ItemType);
};