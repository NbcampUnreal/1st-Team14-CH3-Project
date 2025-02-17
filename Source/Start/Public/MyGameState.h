// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

UCLASS()
class START_API AMyGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMyGameState();
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectCoinCount;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration; // 레벨 제한 시간 (30초)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex; // 현재 진행 중인 레벨 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels; // 전체 레벨 개수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames; // 레벨 이름을 담은 배열

	// 레벨별 아이템 개수 배열 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<int32> ItemsPerLevel; // 레벨별 아이템 개수
	
	//점수를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	// 게임 오버 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	// 초기 레벨 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD() const;
	
};



