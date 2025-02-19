#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

class UCMovementComponent;
class UCWeaponComponent;
class UCCameraComponent;

UCLASS()
class START_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    ACCharacter();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    bool bIsDead;  // 🔹 캐릭터가 사망했는지 여부

public:
    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    void ModifyHealth(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    void Heal(float HealAmount);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UCCameraComponent* CameraComponent;
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UCMovementComponent* MovementComponent;
    UPROPERTY(VisibleAnywhere, Category = "Component")
	UCWeaponComponent* WeaponComponent;
private:
    void Die();  // 🔹 사망 처리 함수
    void SaveHealthToGameInstance();
    void LoadHealthFromGameInstance();
};