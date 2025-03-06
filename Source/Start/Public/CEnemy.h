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
	UFUNCTION(BlueprintCallable)
	void SetRun();
	UFUNCTION(BlueprintCallable)
	void SetWalk();

	UFUNCTION(BlueprintCallable)
	void VisibleEnemyHPBar();
	UFUNCTION(BlueprintCallable)
	void HiddenEnemyHPBar();

	UFUNCTION(BlueprintCallable)
	void SetbCanAttack(bool CanAttack) { bCanAttack = CanAttack; }
	UFUNCTION(BlueprintCallable)
	bool GetbCanAttack() const { return bCanAttack; }

	UFUNCTION(BlueprintCallable)
	ACPlayer* GetHittingPlayer() const {return PlayerHitting;}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadHPWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> BossHPWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* BossHPWidgetInstance;

	UPROPERTY(VisibleAnywhere)
	class UCSpawnComponent* SpawnComp;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyAttack OnEnemyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGunUsed;
	bool bIsDied;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText EnemyName;

	UFUNCTION(BlueprintCallable)
	void UpdateOverheadHP();


	UFUNCTION(BlueprintCallable)
	bool GetIsEnemyDied() const { return bIsDied; }
	UFUNCTION(BlueprintCallable)
	float GetEnemyHP() const;


	UFUNCTION(BlueprintCallable)
	virtual void EnemyAttackStart(bool bIsCloseRangeAttack);
	UFUNCTION(BlueprintCallable)
	virtual void EnemyAttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetIdleMode();
	UFUNCTION(BlueprintCallable)
	void SetEquipMode();
	UFUNCTION(BlueprintCallable)
	void SetActionMode();
	UFUNCTION(BlueprintCallable)
	bool IsEnemyActionMode();
	UFUNCTION(BlueprintCallable)
	bool IsEnemyHitted();

	UFUNCTION(BlueprintCallable)
	void SetStun(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SpawnRandomItemAfterDie();

	void ToDoAfterDie();

	FTimerHandle DieTimerHandle;

	ACPlayer* PlayerHitting;



	virtual void Die() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

};
