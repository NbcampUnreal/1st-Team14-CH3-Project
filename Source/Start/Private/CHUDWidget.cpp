// Fill out your copyright notice in the Description page of Project Settings.

#include "CHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �ʱ� ���� ����
	if (HealthBar)	{HealthBar->SetPercent(1.0f);}
	if (StaminaBar) {StaminaBar->SetPercent(1.0f);}
	if (AmmoText) {AmmoText->SetText(FText::FromString("0 / 0")); }
	if (WeaponNameText)	{WeaponNameText->SetText(FText::FromString("None"));}
	if (ScoreText) {ScoreText->SetText(FText::AsNumber(0));}

	// DamageOverlay�� ItemPickupText�� �⺻������ ���� ó��
	if (DamageOverlay)	{DamageOverlay->SetVisibility(ESlateVisibility::Hidden);}
	if (ItemPickupText){ItemPickupText->SetVisibility(ESlateVisibility::Hidden);}

	// �ʱ� ���¿��� AmmoIcon�� ���� ó��
	if (AmmoIcon) { AmmoIcon->SetVisibility(ESlateVisibility::Hidden); }
}

// ü�� ������Ʈ
void UCHUDWidget::UpdateHealth(float fHealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(fHealthPercent);
	}
}

// ���¹̳� ������Ʈ
void UCHUDWidget::UpdateStamina(float fStaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(fStaminaPercent);
	}
}

// ź�� ���� ������Ʈ
void UCHUDWidget::UpdateAmmo(int32 iCurrentAmmo, int32 iMaxAmmo)
{
	if (AmmoText)
	{
		FString AmmoString = FString::Printf(TEXT("%d / %d"), iCurrentAmmo, iMaxAmmo);
		AmmoText->SetText(FText::FromString(AmmoString));
	}
	// ���� ź���� 0�̸� AmmoIcon�� ���� �� �ֵ���
	if (AmmoIcon)
	{
		AmmoIcon->SetVisibility(iCurrentAmmo > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

// ���� ���� �̸� ������Ʈ
void UCHUDWidget::UpdateWeapon(FText WeaponName)
{
	if (WeaponNameText)
	{
		WeaponNameText->SetText(WeaponName);
	}
}

// ���� ������ ���� ź�� ǥ�� ���� ������Ʈ �Լ�
void UCHUDWidget::UpdateWeaponType(EWeaponType PrevType, EWeaponType NewType)
{
	// ������(Rifle)�� �ǽ���(Pistol)�� �Ѿ� ������ ������.
	bool bShowAmmo = (NewType == EWeaponType::Rifle || NewType == EWeaponType::Pistol);
	SetAmmoVisibility(bShowAmmo);
}

void UCHUDWidget::SetAmmoVisibility(bool bVisible)
{
	if (AmmoText)
	{
		AmmoText->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (AmmoIcon)
	{
		AmmoIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UCHUDWidget::SetCrosshairVisibility(bool bVisible)
{
	if (CrosshairIcon)
	{
		CrosshairIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

// ���� ������Ʈ
void UCHUDWidget::UpdateScore(int32 iNewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(iNewScore));
	}
}

// ���� ȿ�� ǥ��
void UCHUDWidget::ShowDamageEffect()
{
	if (DamageOverlay)
	{
		DamageOverlay->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle TimerHandle;
		// 0.5�� �Ŀ� ȿ�� �����
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (DamageOverlay)
				{
					DamageOverlay->SetVisibility(ESlateVisibility::Hidden);
				}
			}, 0.5f, false);
	}
}

// ������ ȹ�� �˸� ǥ��
void UCHUDWidget::ShowItemPickupNotification(const FText& ItemName)
{
	if (ItemPickupText)
	{
		ItemPickupText->SetText(FText::Format(NSLOCTEXT("HUD", "ItemPickup", "Picked up: {0}"), ItemName));
		ItemPickupText->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle TimerHandle;
		// 2�� �Ŀ� �˸��� ���� ó��
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (ItemPickupText)
				{
					ItemPickupText->SetVisibility(ESlateVisibility::Hidden);
				}
			}, 2.0f, false);
	}
}
