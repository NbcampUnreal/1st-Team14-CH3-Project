#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

struct FInputActionValue;

UENUM()
enum class ESpeedType : uint8
{
	Walk,
	Run,
	Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class START_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45.0f;
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45.0f;
	UPROPERTY(EditAnywhere, category="Speed")
	float Speed[(int32)ESpeedType::Max] = {400.0f, 600.0f};
	
public:
	FORCEINLINE bool GetCanMove() { return bCanMove; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE float GetWarkSpeed() {return Speed[int32(ESpeedType::Walk)];}
	FORCEINLINE float GetRunSpeed() {return Speed[int32(ESpeedType::Run)];}

	FORCEINLINE bool GetControlRotation() { return bControlRotation; }
	FORCEINLINE bool GetFixedCamera() { return bFixednCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixednCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixednCamera = false; }
public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

	void SetSpeed(ESpeedType InType);

public:	
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void EndJump();

	void OnWark();
	void OnRun();
	void EnableControlRotation();
	void DisableControlRoation();

private:
	ACharacter* OwnerCharacter;
	bool bCanMove = true;
	bool bFixednCamera;
	bool bControlRotation = false;
};
