// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "CEnemy.generated.h"

class UWidgetComponent;
class ACPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttack);

UCLASS()
class START_API ACEnemy : public ACCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadHPWidget;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyAttack OnEnemyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGunUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEqueped;
	
	void UpdateOverheadHP();

	UFUNCTION(BlueprintCallable)
	void EnemyAttack();
	UFUNCTION(BlueprintCallable)
	void SetIdleMode();
	UFUNCTION(BlueprintCallable)
	void SetEquipMode();
	UFUNCTION(BlueprintCallable)
	void SetActionMode();

	UFUNCTION(BlueprintCallable)
	void SetRun();
	UFUNCTION(BlueprintCallable)
	void SetWalk();

	UFUNCTION(BlueprintCallable)
	void SetStun(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void Equip();

	UFUNCTION(BlueprintCallable)
	void UnEquip();

	UFUNCTION(BlueprintCallable)
	void ToggleAutoFire();

	UFUNCTION(BlueprintCallable)
	void GunAttackStart();

	UFUNCTION(BlueprintCallable)
	void GunAttackEnd();

	UFUNCTION(BlueprintCallable)
	void DestroyEnemyInMaze();

	UFUNCTION(BlueprintCallable)
	void SpawnRandomItemAfterDie();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

};
