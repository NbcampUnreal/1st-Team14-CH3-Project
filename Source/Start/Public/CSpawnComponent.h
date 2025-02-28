#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorSpawnRow.h"
#include "CSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class START_API UCSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSpawnComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UDataTable* ActorDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	FRotator SpawnRotation;

public:
	FItemSpawnRow* GetRandomActor() const;
	UFUNCTION(BlueprintCallable)
	void SpawnRandomActorToPoint(FVector Location, FRotator Rotation);
};
