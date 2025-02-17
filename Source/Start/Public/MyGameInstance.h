// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


UCLASS()
class START_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GameData")
	int32 CurrentLevelIndex;

	// 점수 총합 함수
	UFUNCTION(BlueprintCallable, Category="GameData")
	void AddToScore(int32 Amount);
};
