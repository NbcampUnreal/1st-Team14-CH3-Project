// Fill out your copyright notice in the Description page of Project Settings.
#include "CBaseItem.h"
#include "Components/SphereComponent.h"

// Sets default values
ACBaseItem::ACBaseItem() :
	bCanUse(false)
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetupAttachment(Scene);


}

void ACBaseItem::PutIntoInventory(AActor* PlayerHavingInventory)
{

}

FName ACBaseItem::GetItemType() const
{
	return ItemType;
}

void ACBaseItem::Use(/*플레이어 클래스*/)
{
}

