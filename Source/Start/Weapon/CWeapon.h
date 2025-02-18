// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

UCLASS(Abstract)
class START_API ACWeapon : public AActor
{
	GENERATED_BODY()

	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void BeginEquip();
	void EndEquip();

	bool CanUnequip();
	void Unequip();

};
