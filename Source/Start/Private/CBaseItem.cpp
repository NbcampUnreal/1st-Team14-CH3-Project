// Fill out your copyright notice in the Description page of Project Settings.
#include "CBaseItem.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

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


}

void ACBaseItem::KeyPressedActivate(AActor* Activator)
{
	PutIntoInventory(Activator); // Inventory를 가져와야 함. 일단 Player 가져오기
	DestroyInteractable();
}

void ACBaseItem::PutIntoInventory(AActor* PlayerHavingInventory)
{
	//인벤토리에 추가
}

FName ACBaseItem::GetItemType() const
{
	return ItemType;
}

void ACBaseItem::Use(/*플레이어 클래스*/)
{
}

