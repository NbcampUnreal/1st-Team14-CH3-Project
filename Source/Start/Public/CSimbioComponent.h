// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSimbioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimbioAttackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimbioActivateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class START_API UCSimbioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCSimbioComponent();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FSimbioAttackDelegate SimbioAttackDelegate;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FSimbioAttackDelegate FSimbioActivateDelegate;

	void SimbioAttack();
	void ActivateSimbio();
	void DeActivateSimbio();
	bool GetIsSimbioActivate() const { return bIsSimbioActivate; }
protected:
	UPROPERTY(EditAnywhere, Category = "Simbio")
	TSubclassOf<AActor> SimbioClass;
	UPROPERTY(VisibleAnywhere, Category = "Simbio")
	AActor* Simbio;
	UPROPERTY(VisibleAnywhere, Category = "Simbio")
	bool bIsSimbioActivate;

	void CreateSimbio();


	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
