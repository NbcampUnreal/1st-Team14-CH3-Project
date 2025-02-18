#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "CHealthPotionItem.generated.h"

UCLASS()
class START_API ACHealthPotionItem : public ACBaseItem
{
	GENERATED_BODY()

public:
	ACHealthPotionItem();

protected:
	virtual void KeyPressedActivate(AActor* Activator) override;
	virtual void Use(/*�÷��̾� Ŭ����*/) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float HealAmount;
};
