// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance_Arms.generated.h"

/**
 * 
 */
UCLASS()
class START_API UCAnimInstance_Arms : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapone")
	EWeaponType WeaponType = EWeaponType::Max;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	ACharacter* OwnerCharacter;
	UCWeaponComponent* Weapon;
};
