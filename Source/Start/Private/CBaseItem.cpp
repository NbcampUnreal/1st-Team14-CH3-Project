// Fill out your copyright notice in the Description page of Project Settings.
#include "CBaseItem.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TestHuman.h" //플레이어로 교체 필요
#include "CItemInventory.h"

// Sets default values
ACBaseItem::ACBaseItem() :
	bCanUse(false)
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(TEXT("BlockAllDynamics"));
	StaticMesh->SetupAttachment(Scene);

	InteractableCollision->SetupAttachment(StaticMesh);
	InteractableCollision->SetCollisionProfileName(TEXT("OvelapAllDynamics"));

	KeyPressedSound = nullptr;
	UseSound = nullptr;
}

void ACBaseItem::KeyPressedActivate(AActor* Activator)
{
	if (KeyPressedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, KeyPressedSound, GetActorLocation());
	}
	PutIntoInventory(Activator); // Inventory를 가져와야 함. 일단 Player 가져오기
	DestroyInteractable();
}

void ACBaseItem::PutIntoInventory(AActor* Player)
{
	ATestHuman* Human = Cast<ATestHuman>(Player);
	if (Human)
	{
		TSharedPtr<CItemInventory> Inventory = Human->GetInventory();
		if (Inventory.IsValid())
		{
			Inventory->AddToInventory(this);
		}
	}
}

EItemType ACBaseItem::GetItemType() const
{
	return ItemType;
}

void ACBaseItem::Use(AActor* Target)
{
	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
	}
}

