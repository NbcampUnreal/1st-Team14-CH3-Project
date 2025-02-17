// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Pawn.h"
#include "SP_Pawn.generated.h"

UCLASS()
class START_API ASP_Pawn : public APawn
{
	GENERATED_BODY()

public:
	ASP_Pawn();
	virtual void Tick(float DeltaTime) override;

	// 루트 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionComp;
	
	// 스켈레탈 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMesh;

	// 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	float MoveRightValue = 0.0f; // MoveRight 입력 값

	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health") //Get 함수 전용 BlueprintPure!
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	// 캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 0.0f;

	//부스터 효과 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystemComponent* BoosterEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UAudioComponent* BoosterSound;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount, // 데미지 양
		struct FDamageEvent const& DamageEvent, // 데미지의 유형, 추가적인 정보(예. 스킬 속성에 따른 데미지)
		class AController* EventInstigator, // 데미지 발생 주체(예. PVP시, 지뢰가 데미지를 입힐 경우 지뢰를 심은 사람)
		AActor* DamageCauser // 데미지 원인 객체, 즉 데미지를 일으킨 오브젝트
		) override;
	
	void Move(const FVector& Direction, float AxisValue);
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MoveUp(const FInputActionValue& Value);  // 상,하 이동 (드론)
	void ActivateBooster(const FInputActionValue& Value);
	void DeactivateBooster(const FInputActionValue& Value);
	void RotateRoll(const FInputActionValue& Value); // Roll 회전 (드론)
	void ToggleFlightHold(const FInputActionValue& Value);
	
	// 중력 및 낙하 관련 함수
	void ApplyGravity(float DeltaTime);
	void CheckGround();
	void ApplyTiltEffect(float DeltaTime);

	// 데미지 시스템 관련
	void OnDeath();
	
private:
	// 이동 속도 및 중력
	const float MoveSpeed = 950.0f;
	const float RotationSpeed = 90.0f;
	const float Gravity = -500.0f;  // 중력 값
	const float Power = 90.0f; // 상승력 (space Bar)
	const float AirControlFactor = 0.9f; // 공중 이동 속도 감소 비율
	const float GroundCheckDistance = 1.0f; // 바닥 감지 거리
	float HoverAltitude = 0.0f;
	
	// 상태 변수
	FVector Velocity = FVector::ZeroVector;  // 현재 속도 벡터
	bool bIsGrounded = false;  // 바닥 감지 여부
	bool bIsFlightHold = false; // 비행 홀드 (CTRL 키)

	bool bIsBoosting = false; // 부스터 상태 여부
	float BoostSpeed = 3000.0f; // 부스터 활성화 속도
};
