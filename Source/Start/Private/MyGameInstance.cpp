// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

