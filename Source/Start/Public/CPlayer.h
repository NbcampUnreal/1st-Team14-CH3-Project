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
	//  �̵� �� ���� ���� �Լ� (UCMovementComponent�� Ȱ��)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();

private:
	//  FPS ī�޶� & �������� ����
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	//  �̵� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMovementComponent* MovementComponent;
};