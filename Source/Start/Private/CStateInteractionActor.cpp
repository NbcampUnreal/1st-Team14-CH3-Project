// Fill out your copyright notice in the Description page of Project Settings.


#include "CStateInteractionActor.h"
#include "CGameState.h"

// Sets default values
ACStateInteractionActor::ACStateInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called every frame
void ACStateInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

