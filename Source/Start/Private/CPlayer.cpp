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
		// ?? ��Ʈ�ѷ� ��������
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

//  �̵� (UCMovementComponent ���)
void ACPlayer::Move(const FInputActionValue& Value)
{
	if (MovementComponent)
	{
		MovementComponent->OnMove(Value);
	}
}

//  ī�޶� ȸ�� (UCMovementComponent ���)
void ACPlayer::Look(const FInputActionValue& Value)
{
	if (MovementComponent)
	{
		MovementComponent->OnLook(Value);
	}
}

//  ���� ���� (UCMovementComponent ���)
void ACPlayer::StartJump()
{
	if (MovementComponent)
	{
		FInputActionValue DummyValue = FInputActionValue(true); // ���� �Է� ���� ��
		MovementComponent->OnJump(DummyValue);
	}
}

//  ���� ����
void ACPlayer::StopJump()
{
	if (MovementComponent)
	{
		MovementComponent->EndJump();
	}
}

//  �޸��� ����
void ACPlayer::StartRun()
{
	if (MovementComponent)
	{
		MovementComponent->OnRun();
	}
}

//  �޸��� ���� (�ȱ�� ����)
void ACPlayer::StopRun()
{
	if (MovementComponent)
	{
		MovementComponent->OnWark();
	}
}
