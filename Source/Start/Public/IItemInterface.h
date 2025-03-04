
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"
#include "FItemData.h"
#include "IItemInterface.generated.h"

// 🔹 아이템 타입 열거형
UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_HealthPotion = 0 UMETA(DisplayName = "HealthPotion"),
    EIT_StaminaPotion = 1 UMETA(DisplayName = "StaminaPotion"),
    EIT_BulletBox = 2 UMETA(DisplayName = "BulletBox"),
    EIT_Bullet = 3 UMETA(DisplayName = "Bullet"),
    EIT_Grenades = 4 UMETA(DisplayName = "Grenades"),
    // 🔫 무기 추가 (더 세분화)
    EIT_Pistol = 5 UMETA(DisplayName = "Pistol"),        // 일반 권총
    EIT_DualPistol = 6 UMETA(DisplayName = "Dual Pistol"),  // 쌍권총
    EIT_Magnum = 7 UMETA(DisplayName = "Magnum"),        // 매그넘 권총 (강력한 한 발)

    EIT_AssaultRifle = 8 UMETA(DisplayName = "Assault Rifle"), // 돌격소총 (AR)
    EIT_BattleRifle = 9 UMETA(DisplayName = "Battle Rifle"),   // 전투소총 (긴 사거리, 높은 데미지)
    EIT_Rifle = 10 UMETA(DisplayName = "Rifle"), // 지정사수 소총 (DMR)
    EIT_Knife = 11 UMETA(DisplayName = "Knife"), //칼
    EIT_None = 255 UMETA(DisplayName = "None")  // ✅ 빈 슬롯 추가
};

// 🔹 블루프린트에서도 사용 가능하도록 인터페이스 추가
UINTERFACE(BlueprintType)
class UIItemInterface : public UInterface
{
    GENERATED_BODY()
};

// 🔹 인터페이스 클래스 (구현해야 할 함수 목록)
class START_API IIItemInterface
{
    GENERATED_BODY()

public:
    // 🔹 아이템을 인벤토리에 추가 (추가 성공 여부 반환)
    virtual bool PutIntoInventory(AActor* PlayerHavingInventory) = 0;

    // 🔹 아이템 사용 기능
    virtual void Use(AActor* Target) = 0;

    // 🔹 아이템 타입 반환
    virtual EItemType GetItemType() const = 0;

    // ✅ `EItemType`을 문자열로 변환하는 정적 함수 추가
    static FString GetItemName(EItemType ItemType)
    {
        switch (ItemType)
        {
        case EItemType::EIT_HealthPotion:
            return TEXT("Health Potion");
        case EItemType::EIT_StaminaPotion:
            return TEXT("Stamina Potion");
        case EItemType::EIT_BulletBox:
            return TEXT("BulletBox");
        case EItemType::EIT_Bullet:
            return TEXT("Bullet");
        case EItemType::EIT_Grenades:
            return TEXT("Grenades");
            // 🔫 무기 추가
        case EItemType::EIT_Pistol:
            return TEXT("Pistol");
        case EItemType::EIT_DualPistol:
            return TEXT("DualPistol");
        case EItemType::EIT_Magnum:
            return TEXT("Magnum");
        case EItemType::EIT_AssaultRifle:
            return TEXT("AssaultRifle");
        case EItemType::EIT_BattleRifle:
            return TEXT("BattleRifle");
        case EItemType::EIT_Rifle:
            return TEXT("Rifle");
        case EItemType::EIT_Knife:
            return TEXT("Knife");
        default:
            return TEXT("Unknown Item");
        }
    }

    // 🔹 올바르게 변환하는 정적 함수 추가
    static FName GetRowNameFromItemType(EItemType ItemType)
    {
        FString EnumString = UEnum::GetValueAsString(ItemType);
        EnumString.RemoveFromStart("EItemType::"); // ✅ "EItemType::" 제거하여 순수 값 반환
        return FName(*EnumString);
    }

    // ✅ 툴팁 정보를 데이터 테이블에서 가져오는 함수
    virtual FString GetItemTooltip(UDataTable* ItemDataTable) const
    {
        if (!ItemDataTable) return TEXT("아이템 데이터 없음");

        FString ContextString;
        FName RowName = *UEnum::GetValueAsString(GetItemType()); // EItemType을 FName으로 변환
        FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, ContextString);

        return ItemData ? ItemData->Description.ToString() : TEXT("툴팁 정보 없음");
    }
};