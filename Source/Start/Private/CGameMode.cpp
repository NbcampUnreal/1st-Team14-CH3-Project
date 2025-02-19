#include "CGameMode.h"
#include "CCharacter.h"
#include "CPlayerController.h"
#include "CGameState.h"

ACGameMode::ACGameMode() 
{
	DefaultPawnClass = ACCharacter::StaticClass();
	PlayerControllerClass = ACPlayerController::StaticClass();
	GameStateClass = ACGameState::StaticClass();
}

