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
		// ?? 컨트롤러 가져오기
		ACPlayerController* PC = Cast<ACPlayerController>(GetController());
		if (PC)
		{
			EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &ACPlayer::Move);
			EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &ACPlayer::Look);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Started, this, &ACPlayer::StartJump);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &ACPlayer::StopJump);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Started, this, &ACPlayer::StartRun);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Completed, this, &ACPlayer::StopRun);
		}
	}
}

//  이동 (UCMovementComponent 사용)
void ACPlayer::Move(const FInputActionValue& Value)
{
	if (MovementComponent)
	{
		MovementComponent->OnMove(Value);
	}
}

//  카메라 회전 (UCMovementComponent 사용)
void ACPlayer::Look(const FInputActionValue& Value)
{
	if (MovementComponent)
	{
		MovementComponent->OnLook(Value);
	}
}

//  점프 시작 (UCMovementComponent 사용)
void ACPlayer::StartJump()
{
	if (MovementComponent)
	{
		FInputActionValue DummyValue = FInputActionValue(true); // 점프 입력 더미 값
		MovementComponent->OnJump(DummyValue);
	}
}

//  점프 종료
void ACPlayer::StopJump()
{
	if (MovementComponent)
	{
		MovementComponent->EndJump();
	}
}

//  달리기 시작
void ACPlayer::StartRun()
{
	if (MovementComponent)
	{
		MovementComponent->OnRun();
	}
}

//  달리기 종료 (걷기로 변경)
void ACPlayer::StopRun()
{
	if (MovementComponent)
	{
		MovementComponent->OnWark();
	}
}
