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
    float PlayerMaxHealth;  // ✅ 최대 체력 저장 추가

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
    int Score;

    FName RespawnTag = NAME_None; // 🔹 리스폰 태그 저장 변수
public:
    void SetRespawnTag(FName NewTag) { RespawnTag = NewTag; }
    FName GetRespawnTag() const { return RespawnTag; }

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void ResetPlayerState(); // 게임오버 시 상태 초기화

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    float GetPlayerHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    float GetPlayerMaxHealth() const { return PlayerMaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void ResetPlayerHealth();  // 체력 초기화 함수

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    int GetScore() const;

    // ✅ 점수 저장 및 불러오기 추가
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void SetScore(int NewScore);
    
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void AddScore(int ScoreAmount);
   
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void ResetScore();  // 점수 초기화 함수

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void NotifyHUDScoreUpdate();

    // ✅ 레벨 변경 시 상태 저장/불러오기
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void SavePlayerState();

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void LoadPlayerState();
};