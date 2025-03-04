#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CGrenadesItem.generated.h"

struct FDoActionData;
class UProjectileMovementComponent;
UCLASS()
class START_API ACGrenadesItem : public ACBaseItem
{
	GENERATED_BODY()
private:
	//UPROPERTY(EditAnywhere, Category = "Action")
	//TArray<FDoActionData> Data;
	UPROPERTY(EditAnywhere, Category = "Item|Component")
	USphereComponent* MineCollision;
	UPROPERTY(EditAnywhere, Category = "Item|Component")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(visibleAnywhere)
	UProjectileMovementComponent* Projectile;
	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMesh; }

public:
	ACGrenadesItem();
	void Shoot(const FVector& InDirection);

protected:
	virtual void BeginPlay() override;
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(AActor* Target) override;
	
	void Explode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDamage;

	FTimerHandle ExplosiveTimerHandle;

};