#include "CPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "CPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "InputActionValue.h"
#include "Weapon/CWeapon.h"
#include "CHUDWidget.h"
#include "GameFramework/HUD.h"
#include "CGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CCameraComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	//  스프링암 설정 (1인칭이므로 길이는 0)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	//  FPS 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCCameraComponent>(TEXT("CameraComponent"));

	// 추가된 SkeletalMesh (Camera에 Attach)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetRelativeLocation(FVector(-14.25f, -5.85f, -156.935f));
	FirstPersonMesh->SetRelativeRotation(FRotator( - 0.5f, -11.85f, -1.2f));
	FirstPersonMesh->SetupAttachment(Camera);
	FirstPersonMesh->SetVisibility(false);  // 비활성화

	bIsFirstPerson = true;

	InventoryComponent = CreateDefaultSubobject<UCInventoryComponent>(TEXT("InventoryComponent"));
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		LastSavedHealth = GetStatusComponent()->GetHealth();
		LastSavedScore = GameInstance->GetScore();
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// ✅ 기존 HUD가 있다면 새로 생성하지 않음
		if (!PC->GetHUD())
		{
			UCHUDWidget* HUDWidget = CreateWidget<UCHUDWidget>(PC, UCHUDWidget::StaticClass());
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
				HUDWidget->BindToPlayer(this);
				UE_LOG(LogTemp, Warning, TEXT("✅ ACPlayer: HUD 위젯 생성 및 바인딩 성공!"));

				if (GameInstance)
				{
					GameInstance->NotifyHUDScoreUpdate(); // ✅ HUD 생성 후 점수 업데이트
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("❌ ACPlayer: HUD 위젯 생성 실패!"));
			}
		}
	}

	CameraComponent->DisableControlRoation();
	//ToggleView(); // 초기 시점 설정

	// ✅ StateComponent 가져오기 (null 체크)
	if (!StateComponent)
	{
		StateComponent = FindComponentByClass<UCStateComponent>();
	}

	if (StateComponent)
	{
		StateComponent->SetIdleMode();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ StateComponent를 찾을 수 없습니다!"));
	}

	// ✅ MovementComponent 가져오기 (null 체크)
	if (!MovementComponent)
	{
		MovementComponent = FindComponentByClass<UCMovementComponent>();
	}

	if (MovementComponent)
	{
		MovementComponent->OnWark(); // ✅ 기본 이동 모드를 '걷기'로 설정
		UE_LOG(LogTemp, Warning, TEXT("🏃‍♂️ 기본 이동 모드: 걷기(Walk)"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ MovementComponent를 찾을 수 없습니다!"));
	}

	// ✅ WeaponComponent 가져오기 (null 체크)
	if (!WeaponComponent)
	{
		WeaponComponent = FindComponentByClass<UCWeaponComponent>();
	}

	if (!WeaponComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ WeaponComponent를 찾을 수 없습니다!"));
	}

	// ✅ 카메라 설정
	if (PC)
	{
		PC->PlayerCameraManager->ViewPitchMin = PitchRange.X;
		PC->PlayerCameraManager->ViewPitchMax = PitchRange.Y;
	}
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
			EnhancedInput->BindAction(PC->ReloadAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::Reload);
			EnhancedInput->BindAction(PC->ToggleAutoFireAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::ToggleAutoFire);
			EnhancedInput->BindAction(PC->ToggleZoomAction, ETriggerEvent::Started, this, &ACPlayer::BeginAim);
			EnhancedInput->BindAction(PC->ToggleZoomAction, ETriggerEvent::Completed, this, &ACPlayer::EndAim);

			EnhancedInput->BindAction(PC->EquipRifleAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::SetRifleMode);
			EnhancedInput->BindAction(PC->EquipPistolAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::SetPistolMode);
			EnhancedInput->BindAction(PC->EquipKnifeAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::SetKnifeMode);
			EnhancedInput->BindAction(PC->ThrowAction, ETriggerEvent::Triggered, WeaponComponent, &UCWeaponComponent::SetGrenadeMode);

			EnhancedInput->BindAction(PC->FireAction, ETriggerEvent::Started, WeaponComponent, &UCWeaponComponent::DoAction);
			EnhancedInput->BindAction(PC->FireAction, ETriggerEvent::Completed, WeaponComponent, &UCWeaponComponent::End_Fire);
			EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &ACPlayer::MoveIfNotInInventory);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Started, this, &ACPlayer::JumpIfNotInInventory);
			// 🔹 시점 전환 액션 바인딩
			EnhancedInput->BindAction(PC->SwitchViewAction, ETriggerEvent::Started, this, &ACPlayer::ToggleView);
		}
	}
}

// ✅ 인벤토리 상태일 때 움직임 방지
void ACPlayer::MoveIfNotInInventory(const FInputActionValue& Value)
{
	if (!StateComponent)
	{
		StateComponent = FindComponentByClass<UCStateComponent>();
	}

	if (StateComponent && StateComponent->GetStateType() == EStateType::Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리 상태에서는 움직일 수 없음!"));
		return;
	}

	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ MovementComponent가 없습니다! 이동할 수 없음."));
		return;
	}

	//MovementComponent->OnMove(Value);
}

void ACPlayer::JumpIfNotInInventory(const FInputActionValue& Value)
{
	if (!StateComponent)
	{
		StateComponent = FindComponentByClass<UCStateComponent>();
	}

	if (StateComponent && StateComponent->GetStateType() == EStateType::Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리 상태에서는 점프할 수 없음!"));
		return;
	}

	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ MovementComponent가 없습니다! 점프할 수 없음."));
		return;
	}

	//MovementComponent->OnJump(Value);
}

void ACPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	if (!StatusComponent) return; // ✅ 멤버 변수 직접 사용

	float CurrentHealth = StatusComponent->GetHealth();
	int CurrentScore = GameInstance->GetScore();

	if (FMath::Abs(LastSavedHealth - CurrentHealth) > KINDA_SMALL_NUMBER || LastSavedScore != CurrentScore)
	{
		GameInstance->SavePlayerState();
		LastSavedHealth = CurrentHealth;
		LastSavedScore = CurrentScore;

		UE_LOG(LogTemp, Warning, TEXT("✅ 상태 변화 감지 - 자동 저장 (체력: %f, 점수: %d)"), CurrentHealth, CurrentScore);
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
		Camera->bUsePawnControlRotation = false;
	}
}

void ACPlayer::BeginAim()
{
	if(WeaponComponent->BeginAim() == false)
		return;
	MovementComponent->OnAim();
}

void ACPlayer::EndAim()
{
	MovementComponent->OnWark();
	WeaponComponent->EndAim();
}