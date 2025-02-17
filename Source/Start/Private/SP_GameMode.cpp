// Fill out your copyright notice in the Description page of Project Settings.
#include "SP_GameMode.h"
//#include "SP_Character.h"
#include "SP_Pawn.h"
#include "SP_PlayerController.h"
#include "MyGameState.h"

ASP_GameMode::ASP_GameMode()
{
	DefaultPawnClass = ASP_Pawn::StaticClass();
	PlayerControllerClass = ASP_PlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
