// Fill out your copyright notice in the Description page of Project Settings.
#include "SP_Pawn.h"
#include "SP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ASP_Pawn::ASP_Pawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 (루트)
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	CollisionComp->SetSimulatePhysics(false);
	RootComponent = CollisionComp;

	// 스켈레탈 메쉬 컴포넌트
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CollisionComp);
	SkeletalMesh->SetSimulatePhysics(false);

	// 스프링 암 컴포넌트
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SkeletalMesh);
	SpringArm->TargetArmLength = 300.0f;
	// Pawn회전과 독립적으로 동작, 왜? 이동 방향을 카메라의 Forward/Right 벡터를 기준으로 계산하여 회전해야 하기 때문에.
	SpringArm->bUsePawnControlRotation = false; 

	// 카메라 컴포넌트
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // 카메라는 스프링 암의 회전만 따르도록 설정

	// 부스터 효과 (파티클 및 사운드)
	BoosterEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BoosterEffect"));
	BoosterEffect->SetupAttachment(SkeletalMesh);
	BoosterEffect->bAutoActivate = false;

	BoosterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BoosterSound"));
	BoosterSound->SetupAttachment(SkeletalMesh);
	BoosterSound->bAutoActivate = false;
	
	Health = MaxHealth;
}

void ASP_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ApplyGravity(DeltaTime);
	CheckGround();
	ApplyTiltEffect(DeltaTime);

	// 부스터 상태에 따른 파티클 효과 업데이트
	if (bIsBoosting)
	{
		if (BoosterEffect && !BoosterEffect->IsActive())
		{
			BoosterEffect->Activate();
		}

		if (BoosterSound && !BoosterSound->IsPlaying())
		{
			BoosterSound->Play();
		}
	}
	else
	{
		if (BoosterEffect && BoosterEffect->IsActive())
		{
			BoosterEffect->Deactivate();
		}

		if (BoosterSound && BoosterSound->IsPlaying())
		{
			BoosterSound->Stop();
		}
	}
}

// 입력 바인딩
void ASP_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
		{
			if (PlayerController->MoveForwardAction)
			EnhancedInput->BindAction(PlayerController->MoveForwardAction, ETriggerEvent::Triggered, this, &ASP_Pawn::MoveForward);

			if (PlayerController->MoveRightAction)
			EnhancedInput->BindAction(PlayerController->MoveRightAction, ETriggerEvent::Triggered, this, &ASP_Pawn::MoveRight);

			if (PlayerController->LookAction)
			EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASP_Pawn::Look);

			if (PlayerController->MoveUpAction)
			EnhancedInput->BindAction(PlayerController->MoveUpAction, ETriggerEvent::Triggered, this, &ASP_Pawn::MoveUp);

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Started, this, &ASP_Pawn::ActivateBooster);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASP_Pawn::DeactivateBooster);
			}

			if (PlayerController->RotateRollAction)
			EnhancedInput->BindAction(PlayerController->RotateRollAction, ETriggerEvent::Triggered, this, &ASP_Pawn::RotateRoll);

			if (PlayerController->FlightHoldAction)
			EnhancedInput->BindAction(PlayerController->FlightHoldAction, ETriggerEvent::Triggered, this, &ASP_Pawn::ToggleFlightHold);
		}
	}
}

void ASP_Pawn::Move(const FVector& Direction, float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		Velocity = FMath::VInterpTo(Velocity, FVector::ZeroVector, GetWorld()->GetDeltaSeconds(), 2.0f);
		return;
	}

	float CurrentSpeed = bIsBoosting ? BoostSpeed : MoveSpeed;
	float SpeedMultiplier = (bIsGrounded && !bIsFlightHold) ? 1.0f : AirControlFactor;
	FVector TargetVelocity = Direction * (AxisValue * CurrentSpeed * SpeedMultiplier);

	// 부드러운 속도 보간
	Velocity = FMath::VInterpTo(Velocity, TargetVelocity, GetWorld()->GetDeltaSeconds(), 6.0f);
}

void ASP_Pawn::MoveForward(const FInputActionValue& Value)
{
	Move(GetActorForwardVector(), Value.Get<float>());
}

void ASP_Pawn::MoveRight(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	
	// A/D 입력값 전달 (왼쪽: -1, 오른쪽: 1, 중립: 0)
	MoveRightValue = AxisValue;
	Move(GetActorRightVector(), Value.Get<float>());
}

void ASP_Pawn::MoveUp(const FInputActionValue& Value)
{
	if (bIsFlightHold) return; // 호버링 상태에서는 수직 이동 입력을 무시하여 높이 고정
	Move(FVector::UpVector, Value.Get<float>());
}

void ASP_Pawn::ActivateBooster(const FInputActionValue& Value)
{
	bIsBoosting = true;
}

void ASP_Pawn::DeactivateBooster(const FInputActionValue& Value)
{
	bIsBoosting = false;
}

void ASP_Pawn::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Yaw 회전 (좌우 회전)
	AddActorLocalRotation(FRotator(0.0f, LookInput.X * RotationSpeed * DeltaTime, 0.0f));

	// Pitch 회전 (카메라 상하 회전)
	FRotator NewSpringArmRot = SpringArm->GetRelativeRotation();
	NewSpringArmRot.Pitch = FMath::ClampAngle(NewSpringArmRot.Pitch + LookInput.Y * RotationSpeed * DeltaTime, -60.0f, 60.0f);
	SpringArm->SetRelativeRotation(NewSpringArmRot);
}

void ASP_Pawn::RotateRoll(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	
	if (FMath::IsNearlyZero(AxisValue)) return;
	
	FRotator CurrentRotation  = GetActorRotation();
	CurrentRotation .Roll += AxisValue * RotationSpeed * GetWorld()->GetDeltaSeconds();

	// Roll 보간 적용
	SetActorRotation(CurrentRotation);
}

// ============ //
// 중력 기능 추가 //
// ============ //
void ASP_Pawn::ApplyGravity(float DeltaTime)
{
	if (bIsFlightHold)
	{
		// 비행 홀드 시 중력 제거, BUT 이동 가능하도록 속도 유지
		Velocity.Z = FMath::FInterpTo(Velocity.Z, 0.0f, DeltaTime, 2.0f);
		//return; : 활성화 하면 호버링 상태로 고정돼버림
	}
    
	if (!bIsGrounded)
	{
		Velocity.Z += Gravity * DeltaTime;
	}
	FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;

	// 호버링 상태이면 수직 위치를 기록된 HoverAltitude로 고정
	if (bIsFlightHold)
	{
		NewLocation.Z = HoverAltitude;
		
	}
	
	// 호버링 상태이면 수직 위치를 고정
	if (bIsFlightHold)
	{
		NewLocation.Z = HoverAltitude;
	}
	
	FHitResult Hit;
	// Sweep 기능으로 이동 시 충돌이 발생하면 Hit에 반영되어 지면에서 멈춥니다.
	SetActorLocation(NewLocation, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		bIsGrounded = true;
		Velocity.Z = 1.0f;
	}
}


void ASP_Pawn::CheckGround()
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, -GroundCheckDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		// 땅에 너무 가까우면 HoverAltitude로 높이를 조정
		if (Hit.Distance < GroundCheckDistance + 10.0f)
		{
			HoverAltitude = Hit.Location.Z + 50.0f; // 땅에서 50 유닛 높이 유지
		}
		bIsGrounded = true;
	}
	else
	{
		bIsGrounded = false;
	}
}

// 틸트 기능!!
void ASP_Pawn::ApplyTiltEffect(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	const float MaxTiltAngle = 15.0f; 
	const float TiltSpeed = 5.0f; // 틸트 반응 속도
	const float RecoverySpeed = 5.5f; // 회복 속도
	const float SineFactor = 0.8f; // 사인 그래프를 조절하는 팩터

	// 비행 홀드 상태면 기울기를 원래 상태로 복구
	if (bIsFlightHold)
	{
		FRotator TargetRotation = CurrentRotation;
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;

		// 사인 그래프 보간을 적용하여 부드럽게 복구
		float Alpha = FMath::Sin(RecoverySpeed * DeltaTime);  
		SetActorRotation(FMath::Lerp(CurrentRotation, TargetRotation, Alpha));
		return;
	}

	// 이동 방향 벡터 정규화
	FVector LocalVelocity = GetActorRotation().UnrotateVector(Velocity).GetSafeNormal();

	// 목표 틸트 각도 계산 (속도가 작을수록 기울기도 작아지도록 보정)
	float TargetPitch = -LocalVelocity.X * MaxTiltAngle;
	float TargetRoll = LocalVelocity.Y * MaxTiltAngle;

	// 이동이 멈추면 원래 자세로 복귀
	if (Velocity.IsNearlyZero())
	{
		TargetPitch = 0.0f;
		TargetRoll = 0.0f;
	}
	
	float Alpha = FMath::Sin(TiltSpeed * DeltaTime * SineFactor);  

	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, TargetPitch, Alpha);
	TargetRotation.Roll = FMath::Lerp(CurrentRotation.Roll, TargetRoll, Alpha);

	SetActorRotation(TargetRotation);
}

// 호버링(비행 홀드) 기능
void ASP_Pawn::ToggleFlightHold(const FInputActionValue& Value)
{
	bIsFlightHold = !bIsFlightHold;
	// 호버링 전환 시 수직 속도를 제거, 현재 높이를 기록하여 고정
	if (bIsFlightHold)
	{
		HoverAltitude = GetActorLocation().Z;
		Velocity.Z = 0.0f;
	}
}

//======================
// 캐릭터 HP, Damage 관련
//======================

float ASP_Pawn::GetHealth() const
{
	return Health;
}

void ASP_Pawn::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health increased to : %f"), Health);
}

float ASP_Pawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                           class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to : %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ASP_Pawn::OnDeath()
{
	// 게임 종료 로직 호출
}