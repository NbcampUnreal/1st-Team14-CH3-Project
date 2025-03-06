// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h" // EWeaponType 사용을 위해 포함
#include "Components/BackgroundBlur.h"
#include "Components/CStatusComponent.h"
#include "CPlayer.h"
#include "CHUDWidget.generated.h"

/**
 *  게임 플레이 중 보여줄 HUD 위젯
 */
UCLASS()
class START_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeHealthBar();
	// 게임 오버 UI 표시
	UFUNCTION(BlueprintCallable)
	void ShowGameOverUI();

    // 버튼 클릭 이벤트
	UFUNCTION(BlueprintCallable)
	void OnReplayClicked();
	void RespawnPlayerAtTaggedSpawnPoint(FName SpawnTag);
	UFUNCTION(BlueprintCallable)
	void OnExitClicked();

	// 체력 업데이트: 0.0f ~ 1.0f 사이의 값
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float fHealthPercent);

	// 스태미너(의지력) 업데이트: 0.0f ~ 1.0f 사이의 값
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float fStaminaPercent);

	// 탄약 정보 업데이트: 현재 탄약과 최대 탄약을 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 iCurrentAmmo, int32 iMaxAmmo);

	// 현재 무기 이름 업데이트
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeapon(FText WeaponName);

	// 점수 업데이트
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScore(int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScoreDisplay();

	// 피해 효과를 표시 (예: 화면 붉게 깜빡임, Blood 비네트 같은 이미지 활용)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageEffect();

	// 아이템 획득 알림 표시 (일정 시간 후 자동으로 사라지도록 설정할 수 있음)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowItemPickupNotification(const FText& ItemName);

	// 무기 종류 변경에 따른 탄약 표시 여부 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeaponType(EWeaponType PrevType, EWeaponType NewType); // 델리게이트용

	// 탄약 표시 여부를 제어하는 함수
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetAmmoVisibility(bool bVisible);

	// 줌인 상태에 따른 크로스헤어 Visibility 제어 함수
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCrosshairVisibility(bool bVisible);

	void BindToPlayer(ACPlayer* Player);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	UCStatusComponent* StatusComponent; // 체력 관리 컴포넌트 참조
	void UpdateHealthBar();
	// UI 요소 바인딩
	UPROPERTY(meta = (BindWidget))
	class UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	// 체력 표시용 ProgressBar (UMG에서 BindWidget)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// 스태미너 표시용 ProgressBar
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	// 탄약 정보를 표시할 TextBlock (예: "10 / 30")
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	// AmmoIcon을 위한 UImage 바인딩
	UPROPERTY(meta = (BindWidget))
	class UImage* AmmoIcon;

	//CrosshairIcon(조준점)을 위한 UImage 바인딩
	UPROPERTY(meta = (BindWidget))
	class UImage* CrosshairIcon;

	// 현재 무기 이름을 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score;

	// 점수를 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	// 피해 효과를 나타내는 오버레이 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* DamageOverlay;

	// 아이템 획득 알림을 표시할 TextBlock
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemPickupText;

	UPROPERTY(meta = (BindWidget))
	UImage* GameoverImage;
	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* GameoverBlur;
};
