// Copyright Epic Games, Inc. All Rights Reserved.

#include "FantasyMissionGameMode.h"

AFantasyMissionGameMode::AFantasyMissionGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFantasyMissionPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Pawns/BP_FloatingCameraPawn"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Controllers/BP_FantasyMissionPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default controller to our Blueprinted GameState
	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBPClass(TEXT("/Game/Controllers/BP_FantasyMissionGameState"));
	if (GameStateBPClass.Class != NULL)
	{
		GameStateClass = GameStateBPClass.Class;
	}

	// set default controller to our Blueprinted HUD
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/Controllers/BP_FantasyMissionHUD"));
	if (GameStateBPClass.Class != NULL)
	{
		HUDClass = HUDBPClass.Class;
	}
	
}