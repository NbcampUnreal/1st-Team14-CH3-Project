// Fill out your copyright notice in the Description page of Project Settings.
#include "SP_Character.h"
#include "SP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASP_Character::ASP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // 스프링 암의 길이를 설정
	SpringArm->bUsePawnControlRotation = true; // 스프링 암이 Pawn의 컨트롤 회전을 따르게 한다.
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // 스프링 암이 따라가주기 때문에 카메라는 이 기능을 꺼줘야 한다.

	NormalSpeed = 800.0f;
	SprintSpeedMultiplier = 5.0f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	JumpCount = 0;
    bIsJumping = false;
}

// Called to bind functionality to input
void ASP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
				PlayerController->MoveAction,
				ETriggerEvent::Triggered,
				this,
				&ASP_Character::Move
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
				PlayerController->JumpAction,
				ETriggerEvent::Triggered,
				this,
				&ASP_Character::StartJump
				);
				EnhancedInput->BindAction(
				PlayerController->JumpAction,
				ETriggerEvent::Completed,
				this,
				&ASP_Character::StopJump
				);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
				PlayerController->SprintAction,
				ETriggerEvent::Triggered,
				this,
				&ASP_Character::StartSprint
				);
				EnhancedInput->BindAction(
				PlayerController->SprintAction,
				ETriggerEvent::Completed,
				this,
				&ASP_Character::StopSprint
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
				PlayerController->LookAction,
				ETriggerEvent::Triggered,
				this,
				&ASP_Character::Look
				);
			}
		}
	}
}

void ASP_Character::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
	AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

}

void ASP_Character::StartJump(const FInputActionValue& Value)
{
	if(Value.Get<bool>() && !bIsJumping)
	{
		Jump();
		bIsJumping = true;
		// 점프 시작 시 로그 출력
        JumpCount++;
        UE_LOG(LogTemp, Log, TEXT("StartJump() Called"));
        GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("Jump Count: %d"), JumpCount));
	}
}
void ASP_Character::StopJump(const FInputActionValue& Value)
{
	if(!Value.Get<bool>())
	{
		StopJumping();
		bIsJumping = false;
		UE_LOG(LogTemp, Log, TEXT("StopJump() Called"));
	}
}

void ASP_Character::StartSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;}
}
void ASP_Character::StopSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;}
}

void ASP_Character::Look(const FInputActionValue& Value)
{
	float Sensitivity = 1.0f;
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X * Sensitivity); // Yaw 회전 (좌우 회전)
	AddControllerPitchInput(LookInput.Y * Sensitivity); // Pitch 회전 (상하 회전)
	
	// Pitch 제한 적용
    if (Controller)
    {
        FRotator Rotation = Controller->GetControlRotation(); // 현재 회전값 가져오기

        // Pitch 값을 제한 (-89도 ~ 89도)
        Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, -60.0f, 60.0f); // 🐞각도 제한은 Clamp가 아니라 ClampAngle을 사용!🐞

        // 제한된 회전값으로 설정
        Controller->SetControlRotation(Rotation);
    }
}
