
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItemInterface.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_HealthPotion UMETA(DispalyName = "HealthPotion"),
	EIT_StaminaPotion UMETA(DispalyName = "StaminaPotion"),
	EIT_Bullet UMETA(DispalyName = "Bullet"),
	EIT_Grenades UMETA(DispalyName = "Grenades")
};

UINTERFACE(MinimalAPI)
class UIItemInterface : public UInterface
{
	GENERATED_BODY()
};

class START_API IIItemInterface
{
	GENERATED_BODY()

public:
	virtual void PutIntoInventory(AActor* PlayerHavingInventory) = 0;
	virtual void Use (AActor* Target) = 0;
	virtual EItemType GetItemType() const = 0;
};
