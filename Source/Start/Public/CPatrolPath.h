// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class START_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Patrol")
	TArray<AActor*> Waypoints;

	AActor* GetWaypoint(int32 index) const;
	int32 Num() const;

};
