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

	// XX�� �ڿ� ���� ������ ��ȯ�ϸ鼭 �Է� ��带 ���� �������� ����
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, [this, World]()
			{
				UGameplayStatics::OpenLevel(World, LevelName);
			}, 1.0f, false);
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

	// ���� ���Ḧ ���� 0.8�� ������ ����
	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, [this, World]()
			{
				APlayerController* PC = GetOwningPlayer();
				UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
			}, 1.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is null, cannot quit game"));
	}
}


