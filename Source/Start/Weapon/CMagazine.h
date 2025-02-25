#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagazine.generated.h"

UCLASS()
class START_API ACMagazine : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh_Full;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh_Empaty;	
public:
	ACMagazine();

protected:
	virtual void BeginPlay() override;

public:
	void SetEject();

private:
	bool bEject;
};
