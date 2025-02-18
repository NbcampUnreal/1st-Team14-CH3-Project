#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

UCLASS()
class START_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    // ü�� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    int Health;

public:
    ACCharacter();

    // HP ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    int GetHP() const;

    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    void SetHP(int HP);

    // ���� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void Attack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void TakeDamaged(int Amount);
};