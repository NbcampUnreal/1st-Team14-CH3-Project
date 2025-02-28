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

	// ��ư ���ε�
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

	// �޴� ������ ���� UI �Է� ���� ���� ��ȯ (���콺 Ŀ���� ���̰� �ϰ� ĳ���� �Է� ����)
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		// ���� ������ ��Ŀ���� �Ҵ�
		InputMode.SetWidgetToFocus(this->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

}

void UCMainMenuWidget::OnStartGameButtonClicked()
{
	// ���� ���� ��û �߻�
	OnStartGameRequested.Broadcast();

	UWorld* World = GetWorld();
	if (World)
	{
		// �ƾ� �������� �Ҵ�Ǿ� �ִٸ� �ƾ� �� ������ �̵�
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
	// ���� ���� ��û �߻�
	OnQuitGameRequested.Broadcast();

	// ���� ���Ḧ ���� 1.0�� ������ ����
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


