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
	UFUNCTION()
	virtual void OnItemOverlap (
		UPrimitiveComponent* OverlapPrimitiveComp, // 오버랩이 발생한 자기 자신 = 구체 콜리전
		AActor* OtherActor, // 콜리전 컴포넌트에 부딪힌 상대 액터 = 플레이어
		UPrimitiveComponent* OtherComp, // 상대 액터의 충돌 컴포넌트 = 플레이어의 충돌 캡슐 컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverlap (
		UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	
	virtual void ActivateItem (AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;
};
