#include "CPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "CPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "InputActionValue.h"

ACPlayer::ACPlayer()
{
	//  스프링암 설정 (1인칭이므로 길이는 0)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.0f; // 1인칭이므로 0
	SpringArm->bUsePawnControlRotation = true;

	//  FPS 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	//  이동 컴포넌트 추가
	MovementComponent = CreateDefaultSubobject<UCMovementComponent>(TEXT("MovementComponent"));
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	//  `ACPlayerController`가 입력 매핑을 관리하므로 별도 설정 불필요
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		// 컨트롤러 가져오기
		ACPlayerController* PC = Cast<ACPlayerController>(GetController());
		if (PC)
		{
			EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, MovementComponent, &UCMovementComponent::OnMove);
			EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, MovementComponent, &UCMovementComponent::OnLook);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Started, MovementComponent, &UCMovementComponent::OnJump);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, MovementComponent, &UCMovementComponent::EndJump);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Started, MovementComponent, &UCMovementComponent::OnRun);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Completed, MovementComponent, &UCMovementComponent::OnWark);
		}
	}
}
