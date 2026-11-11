// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FantasyMission/Objects/Faction.h"
#include "FantasyMission/Objects/Terrain.h"
#include "FantasyMission/Objects/Resource.h"
#include "FantasyMission/Objects/PlayerData.h"
#include "FantasyMission/Objects/Element.h"
#include "FantasyMission/Objects/WorldEvent.h"
#include "FantasyMission/Objects/PlayerMessage.h"
#include "FantasyMission/Actors/MapTile.h"
#include "NavigationSystem.h"
#include <Kismet/GameplayStatics.h>
#include "FantasyMissionGameState.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYMISSION_API AFantasyMissionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFantasyMissionGameState();
public:

	UFUNCTION(BlueprintCallable, category = Event)
	FString SetupPowerUpMessage(UPowerUp* PowerUp);
	
	UFUNCTION(BlueprintCallable, category = PlayerMessage)
	void EndAlliance(FString AggressorName, FString DefendingName);

	UFUNCTION(BlueprintCallable, category = PlayerMessage)
	void RespondToMessage(UPlayerMessage* Message, bool bYes);

	UFUNCTION(BlueprintCallable, category = PlayerMessage)
	void SendMessage(FString Sender, FString Receiver, EPlayerMessageType MessageType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UPlayerMessage*> MessageLine;
	
	UFUNCTION(BlueprintCallable, category = Units)
	void HealUnitsAndLocations(TArray<AFantasyMissionCharacter*> Units, TArray<ALocation*> Locations);

	TMap<AMapTile*, std::pair<EUnitType, int>> ProductionLine;

	UFUNCTION(BlueprintCallable, category = Production)
	TMap<AMapTile*, EUnitType> AdvanceTurnPlayerProductionLine(TArray<ALocation*> Locations);

	UFUNCTION(BlueprintCallable, category = Production)
	void AddNewUnitToProductionLine(AMapTile* MapTile, EUnitType UnitType);

	UFUNCTION(BlueprintCallable, category = Production)
	void RemoveUnitFromProductionLine(AMapTile* MapTile);

	UFUNCTION(BlueprintCallable, category = Factions)
	void LoadFactionData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UFaction*> Factions;

	UFUNCTION(BlueprintCallable, category = Turns)
	void SetTurnTakerIndex(int NewIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UPlayerData*> Players;

	UFUNCTION(BlueprintCallable, category = Factions)
	void SetPlayerFactions(TArray<FString> PlayerNames, TArray<UFaction*> PlayerFactions);

	UFUNCTION(BlueprintCallable, category = Turns)
	void ChangeTurnTaker();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TurnTakerIndex;

	UFUNCTION(BlueprintCallable, category = Turns)
	FString GetCurrentPlayerFaction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> TurnTakerList;

	UFUNCTION(BlueprintCallable, category = Turns)
	void SetTurnTakerList(TArray<FString> TurnTakers);

	UFUNCTION(BlueprintCallable, category = Terrain)
	void CreateTerrains();

	UFUNCTION(BlueprintCallable, category = MapTiles)
	TArray<AMapTile*> GetAdjacentTilesList(AMapTile* StartTile, int Depth);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	TArray<FVector> CalculateMapTileTransforms(FVector StartingPoint, int XAmount, int YAmount);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void SetMapTiles(TArray<AMapTile*> Tiles);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void SetMapTileTerrains();

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void SetResourcesToMapTiles();

	UFUNCTION(BlueprintCallable, category = Attack)
	void DoAttack(UActorSelectionComponent* Attacker, UActorSelectionComponent* Target);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UTerrain*> Terrains;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UElement*> Elements;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> ArcticLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> TundraLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> DesertLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> VarietyLocations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AMapTile*> MapTiles;

	UFUNCTION(BlueprintCallable, category = Resources)
	void RemoveResourceFromTurnPlayer(EResourceType Resource, int Number);

	UFUNCTION(BlueprintCallable, category = Resources)
	void AddResourceToTurnPlayer(EResourceType Resource, int Number);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AFantasyMissionCharacter*> Units;

	UFUNCTION(BlueprintCallable, category = Units)
	void AddUnit(AFantasyMissionCharacter* NewUnit);

	UFUNCTION(BlueprintCallable, category = Units)
	void RemoveUnit(AFantasyMissionCharacter* UnitToRemove);

	bool CheckIfOpponentUnitInMapTile(AMapTile* MapTile, FString FactionID);

	UFUNCTION(BlueprintCallable, category = Elements)
	void CreateElements();

	UFUNCTION(BlueprintCallable, category = Elements)
	UElement* GetElementFromString(FString Name);
	
	void ClearRuin(AFantasyMissionCharacter* Unit, AMapTile* Tile);

	UFUNCTION(BlueprintCallable, category = WorldEvents)
	void StartWorldEvent();

	UFUNCTION(BlueprintCallable, category = WorldEvents)
	void TickWorldEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<AFantasyMissionCharacter*, AMapTile*> UnitInRuin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWorldEvent* CurrentWorldEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWorldEventChanged;

	UFUNCTION(BlueprintCallable, category = WorldEvents)
	void SetupWorldEvent();

	UFUNCTION(BlueprintCallable, category = Diplomacy)
	void AddDefaultRelations(FString PlayerName, TArray<FString> AllPlayerNames);

	UFUNCTION(BlueprintCallable, category = Diplomacy)
	void DeclareWar(FString AggressorName, FString DefendingName);

	UFUNCTION(BlueprintCallable, category = PlayerMessages)
	void ClearEliminatedPlayerMessages();

private:
	FLinearColor CreateLinearColor(FString ColorString);
	//Generation
	static TArray<int> CalculateTerrainPositions(int Amount, int FirstTopIndex, int FirstBottomIndex);
};
