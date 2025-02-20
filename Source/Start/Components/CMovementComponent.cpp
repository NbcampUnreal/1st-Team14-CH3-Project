#include "CMovementComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
{
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnWark()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnMove(const FInputActionValue& Value)
{
	if (OwnerCharacter->GetController() == nullptr)
		return;
	if(bCanMove == false)
		return;
	const FVector2D value = Value.Get<FVector2D>();
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector forward = FQuat(rotator).GetForwardVector();
	FVector right = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(forward, value.X);
	OwnerCharacter->AddMovementInput(right, value.Y);
}

void UCMovementComponent::OnJump(const FInputActionValue& Value)
{
	bool value = Value.Get<bool>();
	if(value == false && OwnerCharacter->GetController() == nullptr && bCanMove == false)
		return;

	OwnerCharacter->Jump();
}

void UCMovementComponent::EndJump()
{
	if(OwnerCharacter->GetController() == nullptr)
		return;
	OwnerCharacter->StopJumping();
}