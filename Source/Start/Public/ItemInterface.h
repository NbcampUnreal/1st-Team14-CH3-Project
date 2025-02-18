// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class START_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual void Use (/*플레이어 클래스*/) = 0;
	virtual FName GetItemType() const = 0;
};
