// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class START_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE bool IsDead() { return Health <= 0.0f; }
public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Damage(float Amount);

private:
	ACharacter* OwnerCharacter;

private:
	float Health;
		
};
