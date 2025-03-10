#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "CInventoryComponent.h"
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
	FORCEINLINE USpringArmComponent* GetSpringArm() const {return SpringArm;}
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
public:
	ACPlayer();
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	UCStatusComponent* GetStatusComponent() const { return StatusComponent; }
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchRange = FVector2D(-40, 40);;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;
protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float StaminaAmount = 0.001;

	void BeginAim();
	void EndAim();

	void SimbioAttack();
	void EndSimbio();
	float LastSavedHealth;
	float LastSavedStamina;
	int LastSavedScore;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCCameraComponent* CameraComponent;
	UPROPERTY()
	UCInventoryComponent* InventoryComponent;

	bool bIsFirstPerson;
	void ToggleView();
	void MoveIfNotInInventory(const FInputActionValue& Value);
	void JumpIfNotInInventory(const FInputActionValue& Value);
	virtual void Tick(float DeltaSeconds) override;
private:
	int32 Count;
};