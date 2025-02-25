// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/CWeapon.h"
#include "Components/CWeaponComponent.h"
#include "Delegates/Delegate.h"
#include "CWeaponInventoryComponent.generated.h"

class UWeaponInventorySlot;

// ✅ 이름 변경: `FWeaponInventoryEntry`
USTRUCT(BlueprintType)
struct FWeaponInventoryEntry
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
    TSubclassOf<ACWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
    UTexture2D* WeaponIcon;

    FWeaponInventoryEntry()
        : WeaponClass(nullptr), WeaponIcon(nullptr) {
    }
};

DECLARE_MULTICAST_DELEGATE(FOnWeaponInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UCWeaponInventoryComponent();

    // ✅ 무기 추가
    bool AddWeaponToInventory(TSubclassOf<ACWeapon> NewWeaponClass, UTexture2D* WeaponIcon);

    // ✅ 무기 드랍
    bool DropWeapon(int32 SlotIndex);

    // ✅ 현재 무기 인벤토리 반환
    const TArray<FWeaponInventoryEntry>& GetWeaponInventory() const { return WeaponInventory; }


    FOnWeaponInventoryUpdated OnWeaponInventoryUpdated;

protected:
    // ✅ 무기 인벤토리 배열 (최대 4개 슬롯)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory")
    TArray<FWeaponInventoryEntry> WeaponInventory;

private:
    TArray<UWeaponInventorySlot*> WeaponSlots;
    const int32 MaxWeaponSlots = 4; // 무기 슬롯 개수 제한
};