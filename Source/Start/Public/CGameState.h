#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CGameInstance.h"
#include "CHUDWidget.h"
#include "CGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    MenuMap UMETA(DisplayName = "메뉴"),
    CityMap UMETA(DisplayName = "도시 맵"),
    MercenaryBattle UMETA(DisplayName = "용병단과 전투"),
    ScoreCheck UMETA(DisplayName = "점수 체크"),
    ElevatorSelection UMETA(DisplayName = "엘리베이터 선택"),
    Labyrinth UMETA(DisplayName = "연구소 미로 맵"),
    BossArea UMETA(DisplayName = "보스 연구소"),
    BossFight UMETA(DisplayName = "보스전"),
    Symbiosis UMETA(DisplayName = "심비오 공생"),
    Ending1 UMETA(DisplayName = "엔딩1: 새로운 인류"),
    Ending2 UMETA(DisplayName = "엔딩2: 자유로 돌아가다"),
    GameOver UMETA(DisplayName = "게임 종료")
};

UCLASS()
class START_API ACGameState : public AGameState
{
	GENERATED_BODY()

public:
    ACGameState();

    // 게임 오버 UI 호출
    UFUNCTION(BlueprintCallable)
    void ShowGameOverUI();
protected:
    virtual void BeginPlay() override;
    // 게임 오버 UI를 위한 클래스 (블루프린트에서 설정 가능)
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<class UCHUDWidget> HUDWidgetClass;

public:
    void SetGameState(EGameState NewState);

    EGameState GetGameState() const { return CurrentState; }

    void CheckScoreForRedDoor();
    void CheckMidBossDefeated();

    FTimerHandle ScoreCheckTimer;
    FTimerHandle MidBossCheckTimer;
private:
    EGameState CurrentState;
    UCHUDWidget* HUDWidgetInstance;
};