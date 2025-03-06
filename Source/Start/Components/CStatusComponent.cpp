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
	Stamina = MaxStamina;
}

void UCStatusComponent::Damage(float Amount)
{
	Health += (Amount * -1);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::UseStamina(float Amount)
{
	Stamina +=(Amount *-1);
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::HealHealth(float Amount)
{
	Health += Amount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::HealStamina(float Amount)
{
	Stamina += Amount;
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}
