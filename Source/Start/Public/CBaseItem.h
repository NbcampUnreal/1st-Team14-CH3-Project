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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Attribute")
	EItemType ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Attribute")
	bool bCanUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	USoundBase* KeyPressedSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	USoundBase* UseSound;



	virtual void Use(AActor* Target) override;
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void PutIntoInventory(AActor* PlayerHavingInventory) override;
	virtual EItemType GetItemType() const override;
	


};
