// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

class UCStateComponent;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "EWeaponType")
	EWeaponType WeaponType = EWeaponType::Max;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bCanMove;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsFalling;
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);


private:
	ACharacter* OwnerCharacter;
	UCharacterMovementComponent* Movement;
	UCStateComponent* State;
	UCWeaponComponent* Weapon;
	FRotator PrevRotation;
};

