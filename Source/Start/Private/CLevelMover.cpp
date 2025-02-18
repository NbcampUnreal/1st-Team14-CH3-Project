// Fill out your copyright notice in the Description page of Project Settings.


#include "CLevelMover.h"
#include "Kismet/GameplayStatics.h"

ACLevelMover::ACLevelMover() :
	LevelToMove(nullptr)
{

}

void ACLevelMover::KeyPressedActivate(AActor* Activator)
{
	if (!LevelToMove.IsNull())
	{
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("CLevelMoverKeyPressed")));
		//UGameplayStatics::OpenLevel(GetWorld(), LevelToMove);
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToMove);
	}//상호작용
	else
	{
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("LevelMover is Null")));
	}
}