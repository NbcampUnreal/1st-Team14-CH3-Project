// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHuman.h"
#include "CItemInventory.h"

// Sets default values
ATestHuman::ATestHuman()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemInventory = MakeShared < CItemInventory>();
}

TSharedPtr< CItemInventory> ATestHuman::GetInventory()
{
	return ItemInventory;
}

// Called when the game starts or when spawned
void ATestHuman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestHuman::Destroyed()
{
}
