// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h" // EWeaponType ����� ���� ����
#include "Components/BackgroundBlur.h"
#include "Components/CStatusComponent.h"
#include "CPlayer.h"
#include "CHUDWidget.generated.h"

/**
 *  ���� �÷��� �� ������ HUD ����
 */
UCLASS()
class START_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeHealthBar();
	// ���� ���� UI ǥ��
	UFUNCTION(BlueprintCallable)
	void ShowGameOverUI();

    // ��ư Ŭ�� �̺�Ʈ
	UFUNCTION(BlueprintCallable)
	void OnReplayClicked();
	void RespawnPlayerAtTaggedSpawnPoint(FName SpawnTag);
	UFUNCTION(BlueprintCallable)
	void OnExitClicked();

	// ü�� ������Ʈ: 0.0f ~ 1.0f ������ ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float fHealthPercent);

	// ���¹̳�(������) ������Ʈ: 0.0f ~ 1.0f ������ ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float fStaminaPercent);

	// ź�� ���� ������Ʈ: ���� ź��� �ִ� ź���� ǥ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 iCurrentAmmo, int32 iMaxAmmo);

	// ���� ���� �̸� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeapon(FText WeaponName);

	// ���� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScore(int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScoreDisplay();

	// ���� ȿ���� ǥ�� (��: ȭ�� �Ӱ� ������, Blood ���Ʈ ���� �̹��� Ȱ��)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageEffect();

	// ������ ȹ�� �˸� ǥ�� (���� �ð� �� �ڵ����� ��������� ������ �� ����)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowItemPickupNotification(const FText& ItemName);

	// ���� ���� ���濡 ���� ź�� ǥ�� ���� ������Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeaponType(EWeaponType PrevType, EWeaponType NewType); // ��������Ʈ��

	// ź�� ǥ�� ���θ� �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetAmmoVisibility(bool bVisible);

	// ���� ���¿� ���� ũ�ν���� Visibility ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCrosshairVisibility(bool bVisible);

	void BindToPlayer(ACPlayer* Player);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	UCStatusComponent* StatusComponent; // ü�� ���� ������Ʈ ����
	void UpdateHealthBar();
	// UI ��� ���ε�
	UPROPERTY(meta = (BindWidget))
	class UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	// ü�� ǥ�ÿ� ProgressBar (UMG���� BindWidget)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// ���¹̳� ǥ�ÿ� ProgressBar
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	// ź�� ������ ǥ���� TextBlock (��: "10 / 30")
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	// AmmoIcon�� ���� UImage ���ε�
	UPROPERTY(meta = (BindWidget))
	class UImage* AmmoIcon;

	//CrosshairIcon(������)�� ���� UImage ���ε�
	UPROPERTY(meta = (BindWidget))
	class UImage* CrosshairIcon;

	// ���� ���� �̸��� ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score;

	// ������ ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	// ���� ȿ���� ��Ÿ���� �������� �̹���
	UPROPERTY(meta = (BindWidget))
	class UImage* DamageOverlay;

	// ������ ȹ�� �˸��� ǥ���� TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemPickupText;

	UPROPERTY(meta = (BindWidget))
	UImage* GameoverImage;
	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* GameoverBlur;
};
