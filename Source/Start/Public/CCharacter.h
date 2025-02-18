#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

UCLASS()
class START_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    // 체력 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    int Health;

public:
    ACCharacter();

    // HP 관련 함수
    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    int GetHP() const;

    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    void SetHP(int HP);

    // 공격 관련 함수
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void Attack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void TakeDamaged(int Amount);
};