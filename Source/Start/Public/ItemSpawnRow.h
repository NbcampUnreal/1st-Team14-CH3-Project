// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase 정의가 들어있는 헤더
#include "ItemSpawnRow.generated.h" // 리플렉션!

USTRUCT(BlueprintType) // 블루프린트에서 인식이 되도록 설정해야 함.
struct FItemSpawnRow : public FTableRowBase 
{
	GENERATED_BODY()
public:
	// 행의 첫 번째는 아이템의 이름이 들어가야 함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName; // 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass; // 아이템의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance; // 스폰 확률
};
