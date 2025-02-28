// Fill out your copyright notice in the Description page of Project Settings.


#include "CCutsceneManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ACutsceneManager::ACutsceneManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACutsceneManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null"));
		return;
	}

	if (CutsceneSequence)
	{
		// �ƾ� �÷��̾� ���� ... �ٵ� �� �ڵ尡 �ȸ�����;; �ϴ� �������Ʈ�� ���
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* OutActor = nullptr;
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(World, CutsceneSequence, PlaybackSettings, OutActor);
		if (SequencePlayer)
		{
			// �ƾ� ��� �Ϸ� �� OnCutsceneFinished ȣ��
			SequencePlayer->OnFinished.AddDynamic(this, &ACutsceneManager::OnCutsceneFinished); //
			SequencePlayer->Play();
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create LevelSequencePlayer."));
			UGameplayStatics::OpenLevel(World, GameMapName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CutsceneSequence is not assigned."));
		UGameplayStatics::OpenLevel(World, GameMapName);
	}
}

// �ƾ� ��� �Ϸ� �� ȣ��� �Լ�
void ACutsceneManager::OnCutsceneFinished()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, GameMapName);
	}
}


