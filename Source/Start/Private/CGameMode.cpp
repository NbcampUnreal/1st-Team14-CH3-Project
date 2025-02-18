#include "CGameMode.h"
#include "CCharacter.h"
#include "CPlayerController.h"

ACGameMode::ACGameMode() 
{
	DefaultPawnClass = ACCharacter::StaticClass();
	PlayerControllerClass = ACPlayerController::StaticClass();
}

