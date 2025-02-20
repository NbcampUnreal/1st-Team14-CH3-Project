// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "CWBP_CInventory.h"
#include "Kismet/GameplayStatics.h"

ACPlayerController::ACPlayerController()
{
	InputMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	JumpAction = nullptr;
	RunAction = nullptr;
	CrouchAction = nullptr;
	SwitchViewAction = nullptr;
	QuitAction = nullptr;

	HUDWidgetClass = nullptr;
	MainMenuWidgetClass = nullptr;
	CurrentWidget = nullptr;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// 현재 레벨 이름에 따라 알맞는 위젯 생성
	UWorld* World = GetWorld();
	if (!World) return;

	FString CurrentLevelName = World->GetMapName();
	UE_LOG(LogTemp, Log, TEXT("Current Level: %s"), *CurrentLevelName);

	// 만약 레벨 이름에 "MenuLevel"가 포함되어 있다면 메인 메뉴 위젯을 생성
	if (CurrentLevelName.Contains(TEXT("MenuLevel")))
	{
		if (MainMenuWidgetClass)
		{
			CurrentWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport(100);

				// 🔹 UI가 초점을 받을 수 있도록 설정
				CurrentWidget->SetIsFocusable(true);

				// 🔹 InputMode 설정 개선
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
				bShowMouseCursor = true;
			}
		}
	}
	else  // 그렇지 않다면 HUD 위젯을 생성
	{
		if (HUDWidgetClass)
		{
			CurrentWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();

				FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				bShowMouseCursor = false;
			}
		}
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &ACPlayerController::ToggleInventory);
	}
}

void ACPlayerController::ToggleInventory()
{
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryWidgetClass가 설정되지 않음!"));
		return;
	}

	if (!InventoryWidget)
	{
		InventoryWidget = CreateWidget<UCWBP_CInventory>(this, InventoryWidgetClass);
		InventoryWidget->AddToViewport();

		if (!InventoryWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ InventoryWidget 생성 실패!"));
			return;
		}
	}

	if (bIsInventoryOpen)
	{
		/*if (InventoryWidget && InventoryWidget->IsInViewport())*/  // 🔹 UI가 화면에 떠 있을 경우
		{
			InventoryWidget->RemoveFromParent();  // 🔹 인벤토리 UI 제거
			/*SetInputMode(FInputModeGameOnly());*/  // 🔹 다시 게임 모드로 변경
			bShowMouseCursor = false;
			UE_LOG(LogTemp, Warning, TEXT("인벤토리 닫기"));
		}
	}
	else
	{
		/*if (InventoryWidget && !InventoryWidget->IsInViewport())*/  // 🔹 UI가 떠 있지 않을 경우
		{
			InventoryWidget->AddToViewport();  // 🔹 인벤토리 UI 화면에 추가
			/*SetInputMode(FInputModeUIOnly());*/  // 🔹 UI 모드로 변경
			bShowMouseCursor = true;
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget 화면에 추가"));
		}
	}

	bIsInventoryOpen = !bIsInventoryOpen;  // 🔹 UI 상태 변경을 가장 마지막에 실행
}

