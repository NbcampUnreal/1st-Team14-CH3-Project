#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
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
	//  이동 및 조작 관련 함수 (UCMovementComponent를 활용)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();

private:
	//  FPS 카메라 & 스프링암 설정
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	//  이동 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMovementComponent* MovementComponent;
};