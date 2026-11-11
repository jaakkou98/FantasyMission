// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FantasyMission/Actors/MapTile.h"
#include "FantasyMission/Components/ActorSelectionComponent.h"
#include "FantasyMission/Components/FactionDataComponent.h"
#include "FantasyMission/Components/UnitDataComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FantasyMission/Objects/HeroData.h"
#include "FantasyMission/Objects/PowerUp.h"
#include "FantasyMissionCharacter.generated.h"

UCLASS(Blueprintable)
class AFantasyMissionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFantasyMissionCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	void MoveUnit(TArray<AMapTile*> NewDestinationPathArray);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHeroData* HeroData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UActorSelectionComponent* ActorSelectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFactionDataComponent* FactionDataComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UUnitDataComponent* UnitDataComponent;

	UFUNCTION(BlueprintCallable, category = Units)
	void ApplyFactionColor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHero;

	UFUNCTION(BlueprintCallable, category = Units)
	void MakeHero();
protected:
	// To add mapping context
	virtual void BeginPlay();

private:
	bool bMove;
	bool bFinishMoving;
	int MovePathIndex;
	TArray<AMapTile*> DestinationPathArray;
	FVector StopLocation;
	void MoveToLocation(AMapTile* Destination);
};

