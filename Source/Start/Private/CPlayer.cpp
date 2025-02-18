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
	//  �������� ���� (1��Ī�̹Ƿ� ���̴� 0)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.0f; // 1��Ī�̹Ƿ� 0
	SpringArm->bUsePawnControlRotation = true;

	//  FPS ī�޶� ����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	//  �̵� ������Ʈ �߰�
	MovementComponent = CreateDefaultSubobject<UCMovementComponent>(TEXT("MovementComponent"));
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	//  `ACPlayerController`�� �Է� ������ �����ϹǷ� ���� ���� ���ʿ�
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		// ��Ʈ�ѷ� ��������
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
