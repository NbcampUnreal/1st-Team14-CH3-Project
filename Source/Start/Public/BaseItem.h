// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractableInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class START_API ABaseItem : public AActor, public IIInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component") // 자체 객체 교체가 아니고, 속성 변환 정도이기에 Visible이면 충분.
	USceneComponent* Scene; // 루트 컴포넌트
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh; // 메시 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* Collision; // 콜리전 컴포넌트
	
	virtual void OnItemOverlap (UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	virtual void OnItemEndOverlap (UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	
	virtual void OverlapActivate(AActor* Activator) override;
	virtual FName GetItemType() const ;

	virtual void DestroyItem(); // 아이템 제거 함수
};
