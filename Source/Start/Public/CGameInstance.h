#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

UCLASS()
class START_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UCGameInstance();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Stamina;
};