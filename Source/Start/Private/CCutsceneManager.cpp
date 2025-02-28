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
		// 컷씬 플레이어 생성 ... 근데 이 코드가 안먹히네;; 일단 블루프린트로 재생
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* OutActor = nullptr;
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(World, CutsceneSequence, PlaybackSettings, OutActor);
		if (SequencePlayer)
		{
			// 컷씬 재생 완료 시 OnCutsceneFinished 호출
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

// 컷씬 재생 완료 시 호출될 함수
void ACutsceneManager::OnCutsceneFinished()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, GameMapName);
	}
}


