// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "CEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class START_API ACEnemy : public ACCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadHPWidget;
	
	void UpdateOverheadHP();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

};
