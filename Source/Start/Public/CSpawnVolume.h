// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActorSpawnRow.h"
#include "CSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class START_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* SpawningBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	class UCSpawnComponent* SpawnComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn") 
	UDataTable* ActorDataTable;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnDelayTime;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int NumToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bShouldSpawnOnce;

	FTimerHandle SpawnTimerHandle;

private:
	//FItemSpawnRow* GetRandomActor() const;
	FVector GetRandomPointInVolume() const;
	//void SpawnActorToPoint(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnActorToRandomPosition();
	void SpawnActorContinuously();
	void SpawnActorOnce(int SpawnNum);

protected:
	virtual void BeginPlay() override;
};
