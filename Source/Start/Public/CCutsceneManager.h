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

	// �ƾ� ��� �Ϸ� �� ȣ��� �Լ�
	UFUNCTION()
	void OnCutsceneFinished();

	// �����Ϳ��� �Ҵ��� �ƾ�(������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene")
	ULevelSequence* CutsceneSequence = nullptr;

	// �ƾ� ��� �Ϸ� �� ��ȯ�� ���� ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene")
	FName GameMapName = FName("GameMap");

private:
	ULevelSequencePlayer* SequencePlayer = nullptr;
};

