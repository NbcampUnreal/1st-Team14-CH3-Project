#include "CSpawnComponent.h"

UCSpawnComponent::UCSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ActorDataTable = nullptr;
}


FItemSpawnRow* UCSpawnComponent::GetRandomActor() const
{
	if (!ActorDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ActorSpawnContext"));
	ActorDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	const float RandValue = FMath::FRandRange(0.f, TotalChance);
	float AccumulatedChance = 0.f;

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


void UCSpawnComponent::SpawnRandomActorToPoint(FVector Location, FRotator Rotation)
{
	SpawnLocation = Location + FVector(0, 0, 10);
	SpawnRotation = Rotation;
	if (FItemSpawnRow* SelectedRow = GetRandomActor())
	{
		if (UClass* ActualClass = SelectedRow->ActorClass.Get())
		{
			GetWorld()->SpawnActor<AActor>(ActualClass, SpawnLocation, SpawnRotation);
		}
	}
}

