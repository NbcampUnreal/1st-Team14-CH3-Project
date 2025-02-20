// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimInstance.h"

#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if(OwnerCharacter == nullptr)
		return;

	Movement = OwnerCharacter->GetCharacterMovement();
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if(State == nullptr || Weapon == nullptr)
		return;

	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(OwnerCharacter == nullptr)
		return;

	Velocity = Movement->Velocity;
	Speed = Velocity.Size2D();

	FRotator rotation = Velocity.ToOrientationRotator();
	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotation, controlRotation);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	bCanMove = Speed > 3.0f && Movement->GetCurrentAcceleration() != FVector::ZeroVector;
	bIsFalling = Movement->IsFalling();
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
