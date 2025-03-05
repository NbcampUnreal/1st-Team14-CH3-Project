// Fill out your copyright notice in the Description page of Project Settings.
#include "CSpawnVolume.h"
#include "CSpawnComponent.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume() :
	SpawnTime(0),
	DelaySpawnTime(0),
	NumToSpawn(10),
	bShouldSpawnOnce(true)
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	SpawnComp = CreateDefaultSubobject<UCSpawnComponent>(TEXT("SpawnComp"));

	//ActorDataTable = nullptr;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (bShouldSpawnOnce)
	{
		SpawnActorOnce(NumToSpawn);
	}
	else
	{
		SpawnActorContinuously();
	}

}

//FItemSpawnRow* ASpawnVolume::GetRandomActor() const
//{
//	if (!ActorDataTable) return nullptr;
//
//	TArray<FItemSpawnRow*> AllRows;
//	static const FString ContextString(TEXT("ActorSpawnContext"));
//	ActorDataTable->GetAllRows(ContextString, AllRows);
//
//	if (AllRows.IsEmpty()) return nullptr;
//
//	float TotalChance = 0.f;
//	for (const FItemSpawnRow* Row : AllRows)
//	{
//		if (Row)
//		{
//			TotalChance += Row->SpawnChance;
//		}
//	}
//	const float RandValue = FMath::FRandRange(0.f, TotalChance);
//	float AccumulatedChance = 0.f;
//
//	for (FItemSpawnRow* Row : AllRows)
//	{
//		AccumulatedChance += Row->SpawnChance;
//		if (RandValue <= AccumulatedChance)
//		{
//			return Row;
//		}
//	}
//	return nullptr;
//}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

//void ASpawnVolume::SpawnActorToPoint(FVector SpawnLocation, FRotator SpawnRotation)
//{
//	if (FItemSpawnRow* SelectedRow = GetRandomActor())
//	{
//		if (UClass* ActualClass = SelectedRow->ActorClass.Get())
//		{
//			GetWorld()->SpawnActor<AActor>(ActualClass, SpawnLocation, SpawnRotation);
//		}
//	}
//
//}

void ASpawnVolume::SpawnActorToRandomPosition()
{
	SpawnComp->SpawnRandomActorToPoint(GetRandomPointInVolume(), FRotator::ZeroRotator);
}

void ASpawnVolume::SpawnActorContinuously()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnVolume::SpawnActorToRandomPosition, SpawnTime, true, DelaySpawnTime);
}

void ASpawnVolume::SpawnActorOnce(int SpawnNum)
{
	for (int i = 0; i < SpawnNum; ++i)
	{
		SpawnActorToRandomPosition();
	}
}





