﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "IItemInterface.h"
#include "FItemData.h"
#include "Components/CStateComponent.h"
#include "CWBP_CInventory.generated.h"

class UCInventoryComponent;
class UCWBP_CInventorySlot;

UCLASS()
class START_API UCWBP_CInventory : public UUserWidget
{
	GENERATED_BODY()

public:
    // 인벤토리 컴포넌트를 초기화하고 이벤트에 바인딩하는 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitializeInventory(UCInventoryComponent* InInventoryComponent);

    // 인벤토리 갱신 함수 (델리게이트와 바인딩)
    UFUNCTION()
    void UpdateInventory();

    void UpdateItemTooltip(IIItemInterface* Item);
    void HideItemTooltip();

    // ✅ 데이터 테이블 참조 (블루프린트에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    UDataTable* ItemDataTable;

    // ✅ UI 요소들
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* T_ItemName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* T_ItemDescription;
  
protected:
    // 인벤토리 컴포넌트 참조
    UPROPERTY()
    UCInventoryComponent* InventoryComponent;

    // 슬롯 위젯 클래스 (블루프린트에서 할당)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<class UCWBP_CInventorySlot> SlotWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryWrapBox;

    // Delegate가 바인딩되어 있는지 여부를 추적하는 플래그
    bool bDelegateBound = false;

    // 위젯 파괴 시 델리게이트 해제를 위한 오버라이드 함수
    virtual void NativeDestruct() override;
};
