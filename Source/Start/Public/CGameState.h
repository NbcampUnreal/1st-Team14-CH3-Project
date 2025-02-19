#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Menu UMETA(DisplayName = "�޴�"),
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

protected:
    virtual void BeginPlay() override;

public:
    void SetGameState(EGameState NewState);

    EGameState GetGameState() const { return CurrentState; }

private:
    EGameState CurrentState;
};