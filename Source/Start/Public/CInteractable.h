
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "IInteractableInterface.h"
#include "CInteractable.generated.h"

UCLASS()
class START_API ACInteractable : public AActor, public IIInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ACInteractable();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable|Component") 
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable|Component")
	class UBoxComponent* InteractableCollision;

	virtual void OnInteractableOverlap(UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnInteractableEndOverlap(UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	virtual void OverlapActivate(AActor* Activator) override;
	virtual void KeyPressedActivate(AActor* Activator) override;

	virtual void DestroyInteractable(); 

};
