#include "CCameraComponent.h"

#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCCameraComponent::UCCameraComponent()
{
}

void UCCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCCameraComponent::OnLook(const FInputActionValue& Value)
{
	if (OwnerCharacter == nullptr || bFixednCamera == true)
		return;
	FVector2d value = Value.Get<FVector2d>();

	if (OwnerCharacter->GetController() == nullptr)
		return;
	OwnerCharacter->AddControllerYawInput(value.X*HorizontalLook*GetWorld()->GetDeltaSeconds());
	OwnerCharacter->AddControllerPitchInput(value.Y*HorizontalLook*GetWorld()->GetDeltaSeconds());
}

void UCCameraComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCCameraComponent::DisableControlRoation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}