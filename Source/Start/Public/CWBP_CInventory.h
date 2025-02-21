// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWBP_CInventory.generated.h"

class UCInventoryComponent;
class UUniformGridPanel;
class UCWBP_CInventorySlot;

UCLASS()
class START_API UCWBP_CInventory : public UUserWidget
{
	GENERATED_BODY()

public:
    // �κ��丮 ������Ʈ�� �ʱ�ȭ�ϰ� �̺�Ʈ�� ���ε��ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitializeInventory(UCInventoryComponent* InInventoryComponent);

    // �κ��丮 ���� �Լ� (��������Ʈ�� ���ε�)
    UFUNCTION()
    void UpdateInventory();

protected:
    // �κ��丮 ������Ʈ ����
    UPROPERTY()
    UCInventoryComponent* InventoryComponent;

    // ���� ���� Ŭ���� (�������Ʈ���� �Ҵ�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<class UCWBP_CInventorySlot> SlotWidgetClass;

    // �κ��丮 �׸���� ��ġ�� �׸��� �г�
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* InventoryGrid;

    // Delegate�� ���ε��Ǿ� �ִ��� ���θ� �����ϴ� �÷���
    bool bDelegateBound = false;

    // ���� �ı� �� ��������Ʈ ������ ���� �������̵� �Լ�
    virtual void NativeDestruct() override;
};
