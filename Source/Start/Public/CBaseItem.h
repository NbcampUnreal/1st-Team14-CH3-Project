// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CInteractable.h"
#include "IItemInterface.h"
#include "CBaseItem.generated.h"

class USphereComponent;

UCLASS()
class START_API ACBaseItem : public ACInteractable, public IIItemInterface
{
	GENERATED_BODY()
	
public:	
	ACBaseItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCanUse;

	virtual void PutIntoInventory(AActor* PlayerHavingInventory) override;
	virtual FName GetItemType() const override;
	virtual void Use(/*플레이어 클래스*/) override;


};
