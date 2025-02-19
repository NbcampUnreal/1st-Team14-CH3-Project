// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
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
				CurrentWidget->AddToViewport();

				// UI 모드 설정: 마우스 커서 활성화, UI 전용 입력 모드 등
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
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