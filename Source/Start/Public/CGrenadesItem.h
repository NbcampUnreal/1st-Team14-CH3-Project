#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "Weapon/CWeaponStructures.h"
#include "CGrenadesItem.generated.h"

struct FDoActionData;
class UProjectileMovementComponent;
UCLASS()
class START_API ACGrenadesItem : public ACBaseItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Item|Component")
	USphereComponent* MineCollision;
	UPROPERTY(EditAnywhere, Category = "Item|Component")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(visibleAnywhere)
	UProjectileMovementComponent* Projectile;
	UPROPERTY(EditDefaultsOnly)
	float LifeTime;
	UPROPERTY(EditDefaultsOnly)
	FHitData HitData;
	UPROPERTY(EditAnywhere)
	UParticleSystem* Particle;
	UPROPERTY(EditDefaultsOnly)
	FVector ParticleScale;
	UPROPERTY(EditDefaultsOnly)
	USoundWave* Sound;

public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMesh; }

public:
	ACGrenadesItem();
	void Shoot(const ACharacter* OwnerCharacter,const FVector& InDirection);
protected:
	virtual void BeginPlay() override;
private:
	void Explode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosiveDamage;

	FTimerHandle ExplosiveTimerHandle;

private:
	TArray<APawn*> Hits;
};