// Fill out your copyright notice in the Description page of Project Settings.


#include "CSimbioComponent.h"
#include "CCharacter.h"

// Sets default values for this component's properties
UCSimbioComponent::UCSimbioComponent() :
	SimbioClass(nullptr),
	Simbio(nullptr),
	bIsSimbioActivate(true)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCSimbioComponent::CreateSimbio()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
		return;
	FTransform SimbioSpawnTrans = OwnerCharacter->GetMesh()->GetSocketTransform("Simbio");
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	if (SimbioClass)
	{
		Simbio = GetWorld()->SpawnActor<AActor>(SimbioClass, SimbioSpawnTrans, SpawnParams);
	}
	if (Simbio)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		Simbio->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, FName("Simbio"));
	}
}

void UCSimbioComponent::ActivateSimbio()
{
	bIsSimbioActivate = true;
	if (!Simbio)
		return;
	Simbio->SetActorHiddenInGame(false);
	Simbio->SetActorEnableCollision(true);
	Simbio->SetActorTickEnabled(true);
	if (FSimbioActivateDelegate.IsBound() == true)
		FSimbioActivateDelegate.Broadcast();
}

void UCSimbioComponent::DeActivateSimbio()
{
	bIsSimbioActivate = false;
	if (!Simbio)
		return;
	Simbio->SetActorHiddenInGame(true);
	Simbio->SetActorEnableCollision(false);
	Simbio->SetActorTickEnabled(false);
}

void UCSimbioComponent::SimbioAttack()
{
	if (SimbioAttackDelegate.IsBound() == true)
		SimbioAttackDelegate.Broadcast();
}

void UCSimbioComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateSimbio();
}

void UCSimbioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

