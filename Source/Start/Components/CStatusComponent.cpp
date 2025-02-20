// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CStatusComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCStatusComponent::UCStatusComponent()
{
}


// Called when the game starts
void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Health = MaxHealth;
	
}

void UCStatusComponent::Damage(float Amount)
{
	Health += (Amount * -1);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
