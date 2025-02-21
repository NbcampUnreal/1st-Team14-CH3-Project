
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItemInterface.generated.h"

// 🔹 아이템 타입 열거형
UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_HealthPotion = 0 UMETA(DisplayName = "HealthPotion"),
    EIT_StaminaPotion = 1 UMETA(DisplayName = "StaminaPotion"),
    EIT_Bullet = 2 UMETA(DisplayName = "Bullet"),
    EIT_Grenades = 3 UMETA(DisplayName = "Grenades")
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
};