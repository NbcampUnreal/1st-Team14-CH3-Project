// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle,
	Equip,
	Action,
	Hitted,
	Dead,
	Inventory,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class START_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsInventoryMode() { return Type == EStateType::Inventory; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EStateType GetStateType() const { return Type; }  // 🔹 StateType을 반환하는 함수 추가

public:	
	UCStateComponent();

	UFUNCTION(BlueprintCallable)
	EStateType GetStateType() const { return Type; }  // 🔹 StateType을 반환하는 함수 추가
protected:
	virtual void BeginPlay() override;

public:	
	void SetIdleMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetInventoryMode();

private:
	void ChangeType(EStateType NewType);
public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType PreveType;
	EStateType Type;
};
