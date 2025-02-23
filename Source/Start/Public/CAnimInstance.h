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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bCanMove;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Yaw;
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

