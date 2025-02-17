// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SP_Character.generated.h"

struct FInputActionValue;
class USpringArmComponent; // 스프링 암 관련 클래스 헤더 미리 선언(전방 선언)
class UCameraComponent; // 스프링 암 관련 클래스 헤더 미리 선언(전방 선언)

UCLASS()
class START_API ASP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASP_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) // 객체 자체 변경은 불가. 내부 속성은 변경 가능
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

protected:
	UPROPERTY()
	int32 JumpCount;
	UPROPERTY()
    bool bIsJumping;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 입력 바인딩 함수는 리플렉션 시스템에 등록 되어 있어야 입력 시스템에서 인지할 수 있다.
	UFUNCTION()
	void Move(const FInputActionValue& Value); // 구조체를 참조자로 하지 않으면 모든 데이터를 복사해오기에 무거워진다. const는 참조된 객체를 수정하지 못하도록 막는다는 취지.
	UFUNCTION()
	void StartJump(const FInputActionValue& Value); //점프와 같이 Bool 타입으로 가져온 동작은 on/off로 나눠주는 편이 좋다.
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	float NormalSpeed;
	float SprintSpeedMultiplier; // 몇 배를 곱해줄 것인지
	float SprintSpeed;
};
