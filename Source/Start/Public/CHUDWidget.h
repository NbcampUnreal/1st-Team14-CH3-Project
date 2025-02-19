// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDWidget.generated.h"

/**
 *  게임 플레이 중 보여줄 HUD 위젯
 */
UCLASS()
class START_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 체력 업데이트: 0.0f ~ 1.0f 사이의 값
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float HealthPercent);

	// 스태미너(의지력) 업데이트: 0.0f ~ 1.0f 사이의 값
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float StaminaPercent);

	// 탄약 정보 업데이트: 현재 탄약과 최대 탄약을 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

	// 현재 무기 이름 업데이트
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeapon(FText WeaponName);

	// 점수 업데이트
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScore(int32 NewScore);

	// 피해 효과를 표시 (예: 화면 붉게 깜빡임, Blood 비네트 같은 이미지 활용)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageEffect();

	// 아이템 획득 알림 표시 (일정 시간 후 자동으로 사라지도록 설정할 수 있음)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowItemPickupNotification(const FText& ItemName);

protected:
	virtual void NativeConstruct() override;

	// 체력 표시용 ProgressBar (UMG에서 BindWidget)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// 스태미너 표시용 ProgressBar
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	// 탄약 정보를 표시할 TextBlock (예: "10 / 30")
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	// 현재 무기 이름을 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	// 점수를 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	// 피해 효과를 나타내는 오버레이 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* DamageOverlay;

	// 아이템 획득 알림을 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemPickupText;
};
