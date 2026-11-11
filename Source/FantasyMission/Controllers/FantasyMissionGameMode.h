// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyMissionGameState.h"
#include "FantasyMissionPlayerController.h"
#include "FantasyMissionHUD.h"
#include "FantasyMission/Pawns/FloatingCameraPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "FantasyMissionGameMode.generated.h"

UCLASS(minimalapi)
class AFantasyMissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFantasyMissionGameMode();
};



