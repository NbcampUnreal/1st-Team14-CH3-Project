#include "CHUDWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"


void UCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기 상태 설정
	if (HealthBar)	{HealthBar->SetPercent(1.0f);}
	if (StaminaBar) {StaminaBar->SetPercent(1.0f);}
	if (AmmoText) {AmmoText->SetText(FText::FromString("0 / 0")); }
	if (WeaponNameText)	{WeaponNameText->SetText(FText::FromString(""));}
	if (ScoreText) {ScoreText->SetText(FText::AsNumber(0));}

	// DamageOverlay와 ItemPickupText는 기본적으로 숨김 처리
	if (DamageOverlay)	{DamageOverlay->SetVisibility(ESlateVisibility::Hidden);}
	if (ItemPickupText){ItemPickupText->SetVisibility(ESlateVisibility::Hidden);}

	// 초기 상태에서 AmmoIcon도 숨김 처리
	if (AmmoIcon) { AmmoIcon->SetVisibility(ESlateVisibility::Hidden); }

	if (GameoverBlur)
	{
		GameoverBlur->SetVisibility(ESlateVisibility::Hidden); // 게임 시작 시 숨김
	}
	if (GameoverImage)
	{
		GameoverImage->SetVisibility(ESlateVisibility::Hidden); // 게임 시작 시 숨기기
	}
	// 버튼 클릭 이벤트 바인딩 (메인 메뉴 버튼 방식과 동일)
	if (ReplayButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReplayButton 바인딩됨"));
		ReplayButton->OnClicked.AddDynamic(this, &UCHUDWidget::OnReplayClicked);
		ReplayButton->SetVisibility(ESlateVisibility::Hidden); // 기본적으로 숨김
	}

	if (ExitButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExitButton 바인딩됨"));
		ExitButton->OnClicked.AddDynamic(this, &UCHUDWidget::OnExitClicked);
		ExitButton->SetVisibility(ESlateVisibility::Hidden); // 기본적으로 숨김
	}

	// 🔹 UI가 완전히 로드될 때까지 기다리도록 타이머 추가
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCHUDWidget::InitializeHealthBar);
}
void UCHUDWidget::InitializeHealthBar()
{
    // 정확한 이름을 사용해야 함 (블루프린트에서 이름 확인 후 수정)
    HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));

    if (HealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ HealthBar 위젯 찾기 성공!"));
        HealthBar->SetPercent(1.0f); // 초기 체력 100%
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ HealthBar 위젯 찾기 실패!"));
    }

    UpdateHealthBar();
}
void UCHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 매 프레임 체력 업데이트
	UpdateHealthBar();
}
void UCHUDWidget::UpdateHealthBar()
{
	// ✅ HealthBar가 nullptr이면 다시 가져오기 시도
	if (!HealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("🔄 HealthBar가 NULL입니다. 다시 가져오겠습니다."));
		HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));

		if (HealthBar)
		{
			UE_LOG(LogTemp, Warning, TEXT("✅ HealthBar 위젯 다시 찾기 성공!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ HealthBar 위젯을 다시 찾을 수 없습니다!"));
			return;
		}
	}

	// ✅ StatusComponent도 다시 확인
	if (!StatusComponent)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
		if (PC)
		{
			ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn());
			if (Player)
			{
				BindToPlayer(Player);
			}
		}
	}

	if (StatusComponent && HealthBar)
	{
		float HealthPercent = StatusComponent->GetHealth() / StatusComponent->GetMaxHealth();
		HealthBar->SetPercent(HealthPercent);		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UCHUDWidget: UpdateHealthBar() 실행 실패 - StatusComponent 또는 HealthBar가 없음"));
	}
}

void UCHUDWidget::BindToPlayer(ACPlayer* Player)
{
	if (Player)
	{
		// ✅ Player에 직접 접근하여 StatusComponent 가져오기
		StatusComponent = Player->GetStatusComponent();

		if (StatusComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("✅ UCHUDWidget: StatusComponent 바인딩 성공!"));
			UpdateHealthBar(); // 초기 체력 UI 업데이트
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ UCHUDWidget: StatusComponent 찾기 실패!"));
		}
	}
}

// 게임 오버 UI 표시
void UCHUDWidget::ShowGameOverUI()
{
	if (GameoverBlur)
	{
		GameoverBlur->SetVisibility(ESlateVisibility::Visible); // ✅ 블러 효과 표시
	}
	if (GameoverImage)
	{
		GameoverImage->SetVisibility(ESlateVisibility::Visible); // ✅ 게임 오버 시 보이도록 설정
	}
	if (ReplayButton)
	{
		ReplayButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ReplayButton->SetIsEnabled(true);  // ✅ 버튼이 비활성화되지 않도록 강제 설정
	}
	if (ExitButton)
	{
		ExitButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ExitButton->SetIsEnabled(true);  // ✅ 버튼이 비활성화되지 않도록 강제 설정
	}

	UE_LOG(LogTemp, Warning, TEXT("게임 오버 UI 버튼, 배경, 블러 활성화됨"));
}

// 게임 재시작 버튼 클릭 시
void UCHUDWidget::OnReplayClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("게임 재시작!"));
	// ✅ 게임오버 배경 다시 숨기기
	if (GameoverImage)
	{
		GameoverImage->SetVisibility(ESlateVisibility::Hidden);
	}
	// 현재 맵 다시 로드
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetMapName()));
}

// 게임 종료 버튼 클릭 시
void UCHUDWidget::OnExitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("게임 종료!"));

	// 게임 종료
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

// 체력 업데이트
void UCHUDWidget::UpdateHealth(float fHealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(fHealthPercent);
	}
}

// 스태미너 업데이트
void UCHUDWidget::UpdateStamina(float fStaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(fStaminaPercent);
	}
}

// 탄약 정보 업데이트
void UCHUDWidget::UpdateAmmo(int32 iCurrentAmmo, int32 iMaxAmmo)
{
	if (AmmoText)
	{
		FString AmmoString = FString::Printf(TEXT("%d / %d"), iCurrentAmmo, iMaxAmmo);
		AmmoText->SetText(FText::FromString(AmmoString));
	}
	// 만약 탄약이 0이면 AmmoIcon도 숨길 수 있도록
	if (AmmoIcon)
	{
		AmmoIcon->SetVisibility(iCurrentAmmo > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

// 현재 무기 이름 업데이트
void UCHUDWidget::UpdateWeapon(FText WeaponName)
{
	if (WeaponNameText)
	{
		WeaponNameText->SetText(WeaponName);
	}
}

// 무기 종류에 따른 탄약 표시 여부 업데이트 함수
void UCHUDWidget::UpdateWeaponType(EWeaponType PrevType, EWeaponType NewType)
{
	// 라이플(Rifle)과 피스톨(Pistol)만 총알 정보를 보여줌.
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

// 점수 업데이트
void UCHUDWidget::UpdateScore(int32 iNewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(iNewScore));
	}
}

// 피해 효과 표시
void UCHUDWidget::ShowDamageEffect()
{
	if (DamageOverlay)
	{
		DamageOverlay->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle TimerHandle;
		// 0.5초 후에 효과 숨기기
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (DamageOverlay)
				{
					DamageOverlay->SetVisibility(ESlateVisibility::Hidden);
				}
			}, 0.5f, false);
	}
}

// 아이템 획득 알림 표시
void UCHUDWidget::ShowItemPickupNotification(const FText& ItemName)
{
	if (ItemPickupText)
	{
		ItemPickupText->SetText(FText::Format(NSLOCTEXT("HUD", "ItemPickup", "Picked up: {0}"), ItemName));
		ItemPickupText->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle TimerHandle;
		// 2초 후에 알림을 숨김 처리
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (ItemPickupText)
				{
					ItemPickupText->SetVisibility(ESlateVisibility::Hidden);
				}
			}, 2.0f, false);
	}
}
