// Fill out your copyright notice in the Description page of Project Settings.
#include "CBaseItem.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "CInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayer.h" 
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
	Use(Activator); //원래는 인벤토리 ui에서 사용해야 함
	
	DestroyInteractable();
}

void ACBaseItem::PutIntoInventory(AActor* Player)
{
	ACPlayer* CPlayer = Cast<ACPlayer>(Player);
	if (CPlayer)
	{
		UCInventoryComponent* Inventory = Player->FindComponentByClass<UCInventoryComponent>();
		if (Inventory)
		{
			Inventory->AddToInventory(GetItemType());
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

