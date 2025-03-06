// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CGameMode.generated.h"

/**
 * 
 */
UCLASS()
class START_API ACGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACGameMode();

	virtual void BeginPlay() override;

	void ChangeLevel(FName LevelName);
    void RestartGame();  // 게임오버 후 재시작 함수
};
