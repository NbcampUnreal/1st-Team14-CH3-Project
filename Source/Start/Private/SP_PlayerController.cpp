// Fill out your copyright notice in the Description page of Project Settings.
#include "SP_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameState.h"
#include "Blueprint/UserWidget.h"
//#include "Components/TextBlock.h"

ASP_PlayerController::ASP_PlayerController()
:	InputMappingContext(nullptr),
	MoveAction(nullptr),
	MoveForwardAction(nullptr),
	MoveRightAction(nullptr),
	SprintAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	MoveUpAction(nullptr),
	RotateRollAction(nullptr),
	FlightHoldAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
	
}

void ASP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem=
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// HUD 생성
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance) HUDWidgetInstance->AddToViewport(); // 뷰포트에 렌더링 해달라
	}
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}

	// 메인 메뉴 위젯 생성 및 추가 (게임 시작 시 최초 표시)
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			// 메뉴 위젯 상호작용을 위해 마우스 커서를 보이게 하고 입력 모드를 UI 전용으로 설정합니다.
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

UUserWidget* ASP_PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}


// void ASP_PlayerController::ShowMainMenu(bool bIsRestart)
// {
// 	//HUD가 켜져 있으면 HUD 닫아주기
// 	if (HUDWidgetInstance)
// 	{
// 		HUDWidgetInstance->RemoveFromParent(); // 생성된 인스턴스(HUD)를 뷰포트로부터 제거
// 		HUDWidgetInstance = nullptr;
// 	}
// 	//이미 메뉴가 켜져 있으면 제거
// 	if (MainMenuWidgetInstance)
// 	{
// 		MainMenuWidgetInstance->RemoveFromParent();
// 		MainMenuWidgetInstance = nullptr;
// 	}
// 	//메인 메뉴 생성
// 	if (MainMenuWidgetInstance)
// 	{
// 		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
// 		if (MainMenuWidgetInstance)
// 		{
// 			MainMenuWidgetInstance->AddToViewport();
// 			// 메인 메뉴가 떠있는 상태에서 캐릭터의 시점 조작을 막기 위한 조치. 무조건 UI로만 가도록
// 			bShowMouseCursor = true; // UI에서 마우스 커서가 보이도록
// 			SetInputMode(FInputModeUIOnly()); // 인풋모드를 UI에서만 사용되도록
// 		}
// 		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
// 		{
// 			if (bIsRestart)
// 			{
// 				ButtonText->SetText(FText::FromString(TEXT("Restart")));
// 			}
// 			else
// 			{
// 				ButtonText->SetText(FText::FromString(TEXT("Start")));
// 			}
// 		}
// 	}
// }