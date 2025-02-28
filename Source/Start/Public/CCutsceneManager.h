// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCutsceneManager.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;

UCLASS()
class START_API ACutsceneManager : public AActor
{
	GENERATED_BODY()

public:
	ACutsceneManager();

protected:
	virtual void BeginPlay() override;

	// 컷씬 재생 완료 후 호출될 함수
	UFUNCTION()
	void OnCutsceneFinished();

	// 에디터에서 할당할 컷씬(시퀀스)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene")
	ULevelSequence* CutsceneSequence = nullptr;

	// 컷씬 재생 완료 후 전환할 게임 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene")
	FName GameMapName = FName("GameMap");

private:
	ULevelSequencePlayer* SequencePlayer = nullptr;
};

