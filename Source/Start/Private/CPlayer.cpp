#include "CPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "CPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "InputActionValue.h"
#include "Components/CCameraComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	//  스프링암 설정 (1인칭이므로 길이는 0)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 100.0f; // 1인칭이므로 0
	SpringArm->bUsePawnControlRotation = true;

	//  FPS 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCCameraComponent>(TEXT("CameraComponent"));

	bIsFirstPerson = true;

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	CameraComponent->DisableControlRoation();
	ToggleView(); // 초기 시점 설정
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
			EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, CameraComponent, &UCCameraComponent::OnLook);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Started, MovementComponent, &UCMovementComponent::OnJump);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, MovementComponent, &UCMovementComponent::EndJump);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Started, MovementComponent, &UCMovementComponent::OnRun);
			EnhancedInput->BindAction(PC->RunAction, ETriggerEvent::Completed, MovementComponent, &UCMovementComponent::OnWark);

			// 🔹 시점 전환 액션 바인딩
			EnhancedInput->BindAction(PC->SwitchViewAction, ETriggerEvent::Started, this, &ACPlayer::ToggleView);
		}
	}
}

void ACPlayer::ToggleView()
{
	bIsFirstPerson = !bIsFirstPerson; // 시점 전환

	if (bIsFirstPerson)
	{
		// 🔹 1인칭(FPS) 설정
		Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Camera->SetRelativeLocation(FVector(0, 0, 80)); // 캐릭터의 눈 위치
		Camera->bUsePawnControlRotation = true;
	}
	else
	{
		// 🔹 3인칭(TPS) 설정
		Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpringArm->TargetArmLength = 300.0f;
		Camera->bUsePawnControlRotation = false;
	}
}
