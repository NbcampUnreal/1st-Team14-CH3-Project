#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CPlayer.h"
#include "CBulletBoxItem.generated.h"


UCLASS()
class START_API ACBulletBoxItem : public ACBaseItem
{
	GENERATED_BODY()
	
public:
	ACBulletBoxItem();
	virtual void Use(AActor* Target) override;

protected:
	virtual void KeyPressedActivate(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int BulletNum;

	bool bIsActivated = false;

	/** 두 번째 스태틱 메쉬 */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* LidMesh;
};
