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
	//  FPS Ä«¸Þ¶ó & ½ºÇÁ¸µ¾Ï ¼³Á¤
=======
	//  ï¿½Ìµï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½ (UCMovementComponentï¿½ï¿½ È°ï¿½ï¿½)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();

private:
	//  FPS Ä«ï¿½Þ¶ï¿½ & ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
>>>>>>> Stashed changes
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

<<<<<<< Updated upstream
	bool bIsFirstPerson;
	void ToggleView();

	//  ÀÌµ¿ ÄÄÆ÷³ÍÆ® Ãß°¡
=======
	//  ï¿½Ìµï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ß°ï¿½
>>>>>>> Stashed changes
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCMovementComponent* MovementComponent;
	UPROPERTY()
	UCWeaponComponent* WeaponComponent;
};