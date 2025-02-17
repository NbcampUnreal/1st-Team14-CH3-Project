// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class START_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn") // 객체를 바꿔야 하므로 EditAny...
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* SpawnRandomItem();
	
	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass); // TSubclassOf 템플릿은 액터의 하위 클래스가 아니면 오류가 발생하게 한다.
	FVector GetRandomPointInVolume() const;

};
