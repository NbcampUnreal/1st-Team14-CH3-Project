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

protected:
    virtual void Init() override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
    float PlayerHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
    int Score;

public:
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    float GetPlayerHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void SetPlayerHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    int GetScore() const;

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void AddScore(int ScoreAmount);
};