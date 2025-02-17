// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get()) // 해당 구조가 TSoftClassPtr -> 소프트 레퍼런스라고 보면 됨.
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const // 누적 랜덤 뽑기 알고리즘 (게임에서 많이 쓰임) 왜 쓰일까? 확률에 비례해서 선택을 보장해준다.
{
	if (!ItemDataTable) return nullptr;

	// 1. 모든 Row(행) 가져오기
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows); // 컨텍스트 스트링은 데이터 테이블에서 행을 가져올 때 디버깅 정보를 제공하는 데 사용된다.

	if (AllRows.IsEmpty()) return nullptr; // 안전 코드

	// 2. 전체 확률 합 구하기
	float TotalChance = 0.f;
	for (const FItemSpawnRow* Row : AllRows) // AllRows 배열의 각 Row 순회
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance; // SpawnChance 값을 TotalChance에 더하기
		}
	}
	// 3. 0 ~ TotalChance 사이의 랜덤 값
	const float RandValue = FMath::FRandRange(0.f, TotalChance);
	float AccumulatedChance = 0.f; // 누적 확률
	
	// 4. 누적 확률로 아이템 선택!
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulatedChance += Row->SpawnChance;
		if (RandValue <= AccumulatedChance)
		{
			return Row;
		}
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	// 박스의 크기를 나타내며, 각 축(X, Y, Z)에 대해 박스의 중심에서 끝까지의 거리를 포함
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// 박스의 중심 좌표
	FVector BoxOrigin = SpawningBox->GetComponentLocation(); 

	// 각 축별로 -Extent ~ Extent 범위의 무작위 값 생성
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
		);
	return SpawnedActor;
}

