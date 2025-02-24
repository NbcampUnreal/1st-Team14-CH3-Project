#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class START_API ACBullet : public AActor
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
public:
	ACBullet();

protected:
	virtual void BeginPlay() override;

public:
	void Shoot(const FVector& InDirection);

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

};

