// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CInteractable.h"
#include "IItemInterface.h"
#include "CBaseItem.generated.h"

class USphereComponent;
class UCInventoryComponent;

UCLASS()
class START_API ACBaseItem : public ACInteractable, public IIItemInterface
{
	GENERATED_BODY()

public:
    ACBaseItem();

    virtual bool PutIntoInventory(AActor* PlayerHavingInventory) override;
    virtual void Use(AActor* Target) override;
    virtual EItemType GetItemType() const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    UTexture2D* ItemIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
    UStaticMeshComponent* StaticMesh;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Attribute")
    EItemType ItemType;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Attribute")
    bool bCanUse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Attribute")
    bool bConsumable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
    USoundBase* KeyPressedSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
    USoundBase* UseSound;
};