﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "CCharacter.generated.h"

class UCFeetComponent;
class UCStateComponent;
class UCStatusComponent;
class UCMontagesComponent;
class UCMovementComponent;
class UCWeaponComponent;
class UCSimbioComponent;

UCLASS()
class START_API ACCharacter : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
    ACCharacter();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    bool bIsHit;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
    bool bIsDead;  // 🔹 캐릭터가 사망했는지 여부

public:
    //UFUNCTION(BlueprintCallable, Category = "Character Stats")
    //float GetHealth() const;
    //UFUNCTION(BlueprintCallable, Category = "Character Stats")
    //float GetMaxHealth() const;

    //UFUNCTION(BlueprintCallable, Category = "Character Stats")
    //void ModifyHealth(float Amount);


    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
    UFUNCTION(BlueprintCallable, Category = "Character Stats")
    void Heal(float HealAmount);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UCStateComponent* StateComponent;
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UCMovementComponent* MovementComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Component")
	UCWeaponComponent* WeaponComponent;
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UCMontagesComponent* MontagesComponent;
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UCStatusComponent* StatusComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UCSimbioComponent* SimbioComponent;
    UFUNCTION(BlueprintCallable)
    virtual void Die();  // 🔹 사망 처리 함수
    void Hitted();


    UFUNCTION()
    void HandleStateChanged(EStateType PreviousType, EStateType NewType);
	UFUNCTION()
    void HandleAnyMontageEnded(UAnimMontage* Montage, bool bInterrupted);
//private:
//    void SaveHealthToGameInstance();
//    void LoadHealthFromGameInstance();
public:
	virtual void End_Hit() override;
	virtual void End_Dead() override;
private:
    struct FHittedInfo
    {
    public:
        struct FActionDamageEvent* Event;
        float Power;
        ACharacter* Character;
        AActor* Causer;

    } HittedInfo;
};