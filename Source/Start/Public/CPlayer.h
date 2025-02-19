#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCMovementComponent;
class ACPlayerController;
struct FInputActionValue;

UCLASS()
class START_API ACPlayer : public ACCharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//  FPS 카메라 & 스프링암 설정
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	bool bIsFirstPerson;
	void ToggleView();

	//  이동 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMovementComponent* MovementComponent;
};