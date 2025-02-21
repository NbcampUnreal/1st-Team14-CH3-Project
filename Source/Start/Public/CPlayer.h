#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CPlayer.generated.h"

class UCCameraComponent;
class UCameraComponent;
class USpringArmComponent;
class ACPlayerController;
struct FInputActionValue;
class UCWeaponComponent;
enum class EWeaponType : uint8;

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
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCCameraComponent* CameraComponent;


	bool bIsFirstPerson;
	void ToggleView();
};