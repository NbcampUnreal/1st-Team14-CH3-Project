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
}

void UCMainMenuWidget::OnStartGameButtonClicked()
{
	// ���� ���� ��û �߻�
	OnStartGameRequested.Broadcast();

	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogTemp, Log, TEXT("Opening level: %s"), *LevelName.ToString());
		UGameplayStatics::OpenLevel(World, LevelName);
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

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = GetOwningPlayer();
		UE_LOG(LogTemp, Log, TEXT("Quitting game"));
		UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is null, cannot quit game"));
	}
}


