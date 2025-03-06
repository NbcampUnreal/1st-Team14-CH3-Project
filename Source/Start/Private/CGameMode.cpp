#include "CGameMode.h"
#include "CCharacter.h"
#include "CPlayerController.h"
#include "CGameState.h"
#include "CGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Components/CStatusComponent.h"

ACGameMode::ACGameMode() 
{
	DefaultPawnClass = ACCharacter::StaticClass();
	PlayerControllerClass = ACPlayerController::StaticClass();
	GameStateClass = ACGameState::StaticClass();
}

void ACGameMode::BeginPlay()
{
    Super::BeginPlay();

    UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        GameInstance->LoadPlayerState(); // ✅ 새로운 맵에서 상태 불러오기
    }
}

void ACGameMode::ChangeLevel(FName LevelName)
{

    UGameplayStatics::OpenLevel(this, LevelName);
}

void ACGameMode::RestartGame()
{
    UCGameInstance* GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GameInstance)
    {
        GameInstance->ResetPlayerState(); // ✅ 체력과 점수 초기화
    }

    // ✅ 현재 맵 다시 로드 (완전 초기화)
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}