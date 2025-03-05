#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CGameInstance.h"
#include "CHUDWidget.h"
#include "CGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    MenuMap UMETA(DisplayName = "�޴�"),
    CityMap UMETA(DisplayName = "���� ��"),
    MercenaryBattle UMETA(DisplayName = "�뺴�ܰ� ����"),
    ScoreCheck UMETA(DisplayName = "���� üũ"),
    ElevatorSelection UMETA(DisplayName = "���������� ����"),
    Labyrinth UMETA(DisplayName = "������ �̷� ��"),
    BossArea UMETA(DisplayName = "���� ������"),
    BossFight UMETA(DisplayName = "������"),
    Symbiosis UMETA(DisplayName = "�ɺ�� ����"),
    Ending1 UMETA(DisplayName = "����1: ���ο� �η�"),
    Ending2 UMETA(DisplayName = "����2: ������ ���ư���"),
    GameOver UMETA(DisplayName = "���� ����")
};

UCLASS()
class START_API ACGameState : public AGameState
{
	GENERATED_BODY()

public:
    ACGameState();

    // ���� ���� UI ȣ��
    UFUNCTION(BlueprintCallable)
    void ShowGameOverUI();
protected:
    virtual void BeginPlay() override;
    // ���� ���� UI�� ���� Ŭ���� (�������Ʈ���� ���� ����)
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