// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class START_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* MineCollision;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDamage;

	FTimerHandle ExplosiveTimerHandle;

	virtual void OverlapActivate(AActor* Activator) override;
	
	void Explode();
};