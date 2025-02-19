#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CPlayer.generated.h"

class UCWeaponComponent;
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
<<<<<<< Updated upstream
	//  FPS ī�޶� & �������� ����
=======
	//  �̵� �� ���� ���� �Լ� (UCMovementComponent�� Ȱ��)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();

private:
	//  FPS ī�޶� & �������� ����
>>>>>>> Stashed changes
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

<<<<<<< Updated upstream
	bool bIsFirstPerson;
	void ToggleView();

	//  �̵� ������Ʈ �߰�
=======
	//  �̵� ������Ʈ �߰�
>>>>>>> Stashed changes
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMovementComponent* MovementComponent;
	UPROPERTY()
	UCWeaponComponent* WeaponComponent;
};