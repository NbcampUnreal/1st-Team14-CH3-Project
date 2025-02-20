// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "CCameraComponent.generated.h"

class ACharacter;
UCLASS()
class START_API UCCameraComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45.0f;
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45.0f;

public:
	FORCEINLINE bool GetControlRotation() { return bControlRotation; }
	FORCEINLINE bool GetFixedCamera() { return bFixednCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixednCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixednCamera = false; }
	
public:
	UCCameraComponent();
protected:
	virtual void BeginPlay() override;
	
public:
	void OnLook(const FInputActionValue& Value);
	void EnableControlRotation();
	void DisableControlRoation();
private:
	ACharacter* OwnerCharacter;
	bool bFixednCamera;
	bool bControlRotation = false;
};
