﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AElevatorTrigger.generated.h"

class UBoxComponent;

UCLASS()
class START_API AAElevatorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAElevatorTrigger();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* BoxComponent;  // 🔹 트리거 박스 (충돌 감지)

    UPROPERTY(EditAnywhere, Category = "Elevator")
    bool bIsN_Elevator;  // 🔹 True면 연구소 미로, False면 보스 연구소

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};