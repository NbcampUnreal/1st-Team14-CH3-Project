#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

class UMaterialInstanceConstant;
class UProjectileMovementComponent;
class UCapsuleComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, AActor*, InCauser, ACharacter*, InOtherCharacter);
UCLASS()
class START_API ACBullet : public ACBaseItem
{
	GENERATED_BODY()
private:
	UPROPERTY(visibleAnywhere)
	UCapsuleComponent* Capsule;
	UPROPERTY(visibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(visibleAnywhere)
	UProjectileMovementComponent* Projectile;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceConstant* Material;
	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

public:
	ACBullet();

protected:
	virtual void BeginPlay() override;

public:
	void Shoot(const FVector& InDirection);

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

public:
	FProjectileHit OnHit;
private:
	TArray<AActor*> Ignores;
};

