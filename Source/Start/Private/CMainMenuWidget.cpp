// Fill out your copyright notice in the Description page of Project Settings.


#include "CMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UCMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 바인딩
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UCMainMenuWidget::OnStartGameButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StartGameButton is not bound!"));
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UCMainMenuWidget::OnQuitClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuitButton is not bound!"));
	}

	// 메뉴 레벨일 때는 UI 입력 전용 모드로 전환 (마우스 커서를 보이게 하고 캐릭터 입력 무시)
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		// 현재 위젯에 포커스를 할당
		InputMode.SetWidgetToFocus(this->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

}

void UCMainMenuWidget::OnStartGameButtonClicked()
{
	// 게임 시작 요청 발생
	OnStartGameRequested.Broadcast();

	UWorld* World = GetWorld();
	if (World)
	{
		// 컷씬 시퀀스가 할당되어 있다면 컷씬 맵 레벨로 이동
		if (CutsceneSequence)
		{
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, [this, World]()
				{
					UGameplayStatics::OpenLevel(World, CutsceneMapName);
				}, 1.0f, false);
			return;
		}
		else
		{
			UGameplayStatics::OpenLevel(World, GameMapName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is null, cannot open level"));
	}
}

void UCMainMenuWidget::OnQuitClicked()
{
	// 게임 종료 요청 발생
	OnQuitGameRequested.Broadcast();

	// 게임 종료를 위해 1.0초 딜레이 적용
	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, [this, World]()
			{
				APlayerController* PC = GetOwningPlayer();
				UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
			}, 0.8f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is null, cannot quit game"));
	}
}


