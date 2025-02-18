// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class START_API AMineItem : public ACBaseItem
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

	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(/*플레이어 클래스*/) override;
	
	void Explode();
};