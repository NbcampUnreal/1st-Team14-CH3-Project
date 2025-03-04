// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CFeetComponent.h"
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
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EWeaponType")
	EWeaponType WeaponType = EWeaponType::Max;

protected:
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	bool bInAim;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	bool bUseHandIk;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	FVector LeftHandLocation;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	FVector LeftHandAimLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
	UCFeetComponent* FeetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
	bool bFeet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
	FFeetData FeetData;

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

