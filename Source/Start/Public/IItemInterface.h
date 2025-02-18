
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItemInterface.generated.h"

// This class does not need to be modified.
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
	virtual void Use (/*플레이어 클래스*/) = 0;
	virtual FName GetItemType() const = 0;
};
