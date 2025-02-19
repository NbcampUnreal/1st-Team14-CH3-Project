// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDWidget.generated.h"

/**
 *  ���� �÷��� �� ������ HUD ����
 */
UCLASS()
class START_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ü�� ������Ʈ: 0.0f ~ 1.0f ������ ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float HealthPercent);

	// ���¹̳�(������) ������Ʈ: 0.0f ~ 1.0f ������ ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float StaminaPercent);

	// ź�� ���� ������Ʈ: ���� ź��� �ִ� ź���� ǥ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

	// ���� ���� �̸� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeapon(FText WeaponName);

	// ���� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScore(int32 NewScore);

	// ���� ȿ���� ǥ�� (��: ȭ�� �Ӱ� ������, Blood ���Ʈ ���� �̹��� Ȱ��)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageEffect();

	// ������ ȹ�� �˸� ǥ�� (���� �ð� �� �ڵ����� ��������� ������ �� ����)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowItemPickupNotification(const FText& ItemName);

protected:
	virtual void NativeConstruct() override;

	// ü�� ǥ�ÿ� ProgressBar (UMG���� BindWidget)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// ���¹̳� ǥ�ÿ� ProgressBar
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	// ź�� ������ ǥ���� TextBlock (��: "10 / 30")
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	// ���� ���� �̸��� ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	// ������ ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	// ���� ȿ���� ��Ÿ���� �������� �̹���
	UPROPERTY(meta = (BindWidget))
	class UImage* DamageOverlay;

	// ������ ȹ�� �˸��� ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemPickupText;
};
