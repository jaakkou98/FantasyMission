// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyMissionGameState.h"
#include "HAL/PlatformFileManager.h"
#include "FantasyMission/Controllers/FantasyMissionPlayerController.h"

AFantasyMissionGameState::AFantasyMissionGameState()
{
	ProductionLine = TMap<AMapTile*, std::pair<EUnitType, int>>();
	UnitInRuin = TMap<AFantasyMissionCharacter*, AMapTile*>();
}

void AFantasyMissionGameState::ClearEliminatedPlayerMessages()
{
	TArray<UPlayerMessage*> MessagesToRemove = TArray<UPlayerMessage*>();
	for (UPlayerMessage* TempMessage : MessageLine)
	{
		if (TempMessage->Receiver == TurnTakerList[TurnTakerIndex] || TempMessage->Sender == TurnTakerList[TurnTakerIndex])
		{
			MessagesToRemove.Add(TempMessage);
		}
	}
	for (UPlayerMessage* TempMessage : MessagesToRemove)
	{
		MessageLine.Remove(TempMessage);
	}
}

FString AFantasyMissionGameState::SetupPowerUpMessage(UPowerUp* PowerUp)
{
	FString Description = "";
	FString ActionString = "";
	if (PowerUp->Type == EPowerUpType::Action)
	{
		TArray<FString> StringArray = TArray<FString>();
		ActionString = UEnum::GetValueAsString(PowerUp->Action);
		ActionString.ParseIntoArray(StringArray, TEXT(":"), true);
		ActionString = StringArray[1];
	}
	switch (PowerUp->Type)
	{
	case EPowerUpType::Hero:
		Description = "New Hero\n\nUnit became a hero!";
		break;
	case EPowerUpType::Action:
		Description = "New Action\n\nUnit learned " + ActionString + "!";
		break;
	case EPowerUpType::LevelUp:
		Description = "Level Up\n\nUnit gained a level!";
		break;
	case EPowerUpType::StatBoost:
		FString Stat = PowerUp->StatBoost;
		Description = Stat + " Boost\n\nUnit's " + Stat + " increased!";
		break;
	}

	return Description;
}

void AFantasyMissionGameState::RespondToMessage(UPlayerMessage* Message, bool bYes)
{
	UPlayerData* Responder = Players[Message->Receiver];
	UPlayerData* Receiver = Players[Message->Sender];

	switch (Message->Type)
	{
	case EPlayerMessageType::WarEnd:
		if (bYes)
		{
			Responder->Relations[Message->Sender] = EPlayerRelation::Neutral;
			Receiver->Relations[Message->Receiver] = EPlayerRelation::Neutral;
			SendMessage(Message->Receiver, Message->Sender, EPlayerMessageType::PeaceAccepted);
		}
		else
		{
			SendMessage(Message->Receiver, Message->Sender, EPlayerMessageType::PeaceRejected);
		}
		break;
	case EPlayerMessageType::AllianceStart:
		if (bYes)
		{
			Responder->Relations[Message->Sender] = EPlayerRelation::Ally;
			Receiver->Relations[Message->Receiver] = EPlayerRelation::Ally;
			SendMessage(Message->Receiver, Message->Sender, EPlayerMessageType::AllianceAccepted);
		}
		else
		{
			SendMessage(Message->Receiver, Message->Sender, EPlayerMessageType::AllianceRejected);
		}
		break;
	default:
		UE_LOG(LogTemp, Display, TEXT("Message Type is probably wrong somewhere"));
		break;
	}
}

void AFantasyMissionGameState::EndAlliance(FString AggressorName, FString DefendingName)
{
	UPlayerData* Aggressor = Players[AggressorName];
	UPlayerData* Defender = Players[DefendingName];

	Aggressor->Relations[DefendingName] = EPlayerRelation::Neutral;
	Defender->Relations[AggressorName] = EPlayerRelation::Neutral;

	SendMessage(AggressorName, DefendingName, EPlayerMessageType::AllianceEnd);
}

void AFantasyMissionGameState::DeclareWar(FString AggressorName, FString DefendingName)
{
	UPlayerData* Aggressor = Players[AggressorName];
	UPlayerData* Defender = Players[DefendingName];

	Aggressor->Relations[DefendingName] = EPlayerRelation::Enemy;
	Defender->Relations[AggressorName] = EPlayerRelation::Enemy;

	SendMessage(AggressorName, DefendingName, EPlayerMessageType::WarStart);
}

void AFantasyMissionGameState::SendMessage(FString Sender, FString Receiver, EPlayerMessageType MessageType)
{
	UPlayerMessage* NewMessage = NewObject<UPlayerMessage>();
	NewMessage->SetMessageType(MessageType);
	NewMessage->SetMessageValues(Sender, Receiver);

	MessageLine.Add(NewMessage);
}

void AFantasyMissionGameState::SetupWorldEvent()
{
	CurrentWorldEvent = NewObject<UWorldEvent>();
	CurrentWorldEvent->SetEventType(EWorldEventType::NoBoost);
	CurrentWorldEvent->SetEventValues(CurrentWorldEvent);
	bWorldEventChanged = false;
}

void AFantasyMissionGameState::TickWorldEvent()
{
	CurrentWorldEvent->Turns -= 1;

	UE_LOG(LogTemp, Display, TEXT("World Event turns left %s"), *FString::FromInt(CurrentWorldEvent->Turns));
	if (CurrentWorldEvent->Turns == 0)
	{
		StartWorldEvent();
	}
}

void AFantasyMissionGameState::StartWorldEvent()
{
	TArray<EWorldEventType> PossibleWorldEvents = TArray<EWorldEventType>();
	for (EWorldEventType TempEvent : TEnumRange<EWorldEventType>())
	{
		if (CurrentWorldEvent->Type == EWorldEventType::NoBoost || CurrentWorldEvent->Type == EWorldEventType::EXPBoost)
		{
			if (TempEvent != CurrentWorldEvent->Type)
			{
				PossibleWorldEvents.Add(TempEvent);
			}
		}
		else
		{
			PossibleWorldEvents.Add(TempEvent);
		}
	}
	EWorldEventType NewType = PossibleWorldEvents[FMath::RandRange(0, PossibleWorldEvents.Num() - 1)];
	UE_LOG(LogTemp, Display, TEXT("New World Event %s"), *UEnum::GetValueAsName(NewType).ToString());
	UWorldEvent* NewEvent = NewObject<UWorldEvent>();
	NewEvent->SetEventType(NewType);
	NewEvent->SetEventValues(CurrentWorldEvent);
	CurrentWorldEvent = NewEvent;
	UE_LOG(LogTemp, Display, TEXT("World Event %s starts"), *UEnum::GetValueAsName(CurrentWorldEvent->Type).ToString());
	bWorldEventChanged = true;
}

void AFantasyMissionGameState::ClearRuin(AFantasyMissionCharacter* Unit, AMapTile* Tile)
{
	UnitInRuin.Add(Unit, Tile);
}

void AFantasyMissionGameState::HealUnitsAndLocations(TArray<AFantasyMissionCharacter*> UnitList, TArray<ALocation*> LocationList)
{
	for (ALocation* TempLocation : LocationList)
	{
		if (TempLocation->FactionDataComponent->FactionData->FactionID == GetCurrentPlayerFaction())
		{
			TempLocation->Heal(TempLocation->LocationLevel);
			for (AFantasyMissionCharacter* TempUnit : UnitList)
			{
				if (TempUnit->FactionDataComponent->FactionData->FactionID == GetCurrentPlayerFaction())
				{
					if (TempLocation->Territory.Contains(TempUnit->ActorSelectionComponent->CurrentMapTile))
					{
						TempUnit->UnitDataComponent->UnitData->Heal(TempLocation->LocationLevel);
					}
				}
			}
		}
	}
}

void AFantasyMissionGameState::DoAttack(UActorSelectionComponent* Attacker, UActorSelectionComponent* Target)
{
	FString Aggressor = TurnTakerList[TurnTakerIndex];
	FString Defender = "";
	UAction* UsedAction = Cast<AFantasyMissionPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->UsedAction;
	if (AFantasyMissionCharacter* AttackerUnit = Cast<AFantasyMissionCharacter>(Attacker->GetOwner()))
	{
		if (AttackerUnit->UnitDataComponent->ActionsAvailable >= UsedAction->ActionCost)
		{
			int AttackModifier = AttackerUnit->UnitDataComponent->UnitData->AttackModifier;

			EActionElement AttackElement = UsedAction->Element;
			EActionElement AttackerStrength = EActionElement::None;
			EActionElement AttackerWeakness = EActionElement::None;

			for (UElement* TempElement : Elements)
			{
				if (TempElement->Type == AttackElement)
				{
					AttackerStrength = TempElement->Strength;
					AttackerWeakness = TempElement->Weakness;
				}
			}

			int WorldEventModifier = 1;
			if (CurrentWorldEvent->Type == EWorldEventType::ElementBoost)
			{
				if (CurrentWorldEvent->ElementType == AttackElement)
				{
					WorldEventModifier = 2;
				}
			}
			else if (CurrentWorldEvent->Type == EWorldEventType::UnitBoost)
			{
				if (CurrentWorldEvent->UnitType == AttackerUnit->UnitDataComponent->UnitData->Type)
				{
					WorldEventModifier = 2;
				}
			}

			int Damage = 0;
			int Health = 0;
			bool bDidDamage = false;
			float RandomNumber = FMath::FRandRange(0.8, 1.8);
			if (AFantasyMissionCharacter* TargetUnit = Cast<AFantasyMissionCharacter>(Target->GetOwner()))
			{
				if (TargetUnit->FactionDataComponent->FactionData->FactionID != AttackerUnit->FactionDataComponent->FactionData->FactionID)
				{
					for (FString Name : TurnTakerList)
					{
						if (Players[Name]->Faction->FactionID == TargetUnit->FactionDataComponent->FactionData->FactionID)
						{
							Defender = Name;
						}
					}
					if (Players[Aggressor]->Relations[Defender] == EPlayerRelation::Ally)
					{
						EndAlliance(Aggressor, Defender);
					}
					if (Players[Aggressor]->Relations[Defender] != EPlayerRelation::Enemy)
					{
						DeclareWar(Aggressor, Defender);
					}
					EActionElement DefenderElement = TargetUnit->UnitDataComponent->Element;
					bDidDamage = true;
					int DefenseModifier = TargetUnit->UnitDataComponent->UnitData->DefenseModifier;
					float AttackMultiplier = 1;
					if (DefenderElement == AttackerStrength)
					{
						AttackMultiplier = 2;
					}
					else if (DefenderElement == AttackerWeakness)
					{
						AttackMultiplier = 0.5;
					}
					Damage = std::round((AttackMultiplier*AttackModifier*WorldEventModifier - DefenseModifier)*RandomNumber);
					if (Damage <= 0)
					{
						Damage = 1;
					}
					TargetUnit->UnitDataComponent->UnitData->TakeDamage(Damage);
					Health = TargetUnit->UnitDataComponent->UnitData->Health;
					if (Health == 0)
					{
						int AttackerLevel = AttackerUnit->UnitDataComponent->UnitData->Level;
						//gain exp
						bool Boost = false;
						if (CurrentWorldEvent->Type == EWorldEventType::EXPBoost)
						{
							Boost = true;
						}
						AttackerUnit->UnitDataComponent->UnitData->GainExperience(TargetUnit->UnitDataComponent->UnitData->GetEXPYield(), Boost);
						//see if leveled up
						if (AttackerLevel != AttackerUnit->UnitDataComponent->UnitData->Level)
						{
							//chance to become hero
							if (!AttackerUnit->bHero)
							{
								float RandomChance = FMath::FRand();
								if (RandomChance < 0.01)
								{
									AttackerUnit->MakeHero();
								}
							}
						}
						//kill unit
						TargetUnit->bDead = true;
					}
				}
			}
			else if (ALocation* TargetLocation = Cast<ALocation>(Target->GetOwner()))
			{
				if (TargetLocation->FactionDataComponent->FactionData->FactionID != AttackerUnit->FactionDataComponent->FactionData->FactionID)
				{
					for (FString Name : TurnTakerList)
					{
						if (Players[Name]->Faction->FactionID == TargetLocation->FactionDataComponent->FactionData->FactionID)
						{
							Defender = Name;
						}
					}
					if (Players[Aggressor]->Relations[Defender] == EPlayerRelation::Ally)
					{
						EndAlliance(Aggressor, Defender);
					}
					if (Players[Aggressor]->Relations[Defender] != EPlayerRelation::Enemy)
					{
						DeclareWar(Aggressor, Defender);
					}
					EActionElement DefenderElement = TargetLocation->Element;
					bDidDamage = true;
					int DefenseModifier = TargetLocation->DefenseModifier;
					float AttackMultiplier = 1;
					if (DefenderElement == AttackerStrength)
					{
						AttackMultiplier = 2;
					}
					else if (DefenderElement == AttackerWeakness)
					{
						AttackMultiplier = 0.5;
					}
					Damage = std::round((AttackMultiplier*AttackModifier * WorldEventModifier - DefenseModifier)*RandomNumber);
					if (Damage <= 0)
					{
						Damage = 1;
					}
					TargetLocation->TakeDamage(Damage);
					Health = TargetLocation->Health;
					if (Health == 0)
					{
						int AttackerLevel = AttackerUnit->UnitDataComponent->UnitData->Level;
						//Gain exp
						bool Boost = false;
						if (CurrentWorldEvent->Type == EWorldEventType::EXPBoost)
						{
							Boost = true;
						}
						AttackerUnit->UnitDataComponent->UnitData->GainExperience(TargetLocation->GetEXPYield(), Boost);
						//see if leveled up
						if (AttackerLevel != AttackerUnit->UnitDataComponent->UnitData->Level)
						{
							//chance to become hero
							if (!AttackerUnit->bHero)
							{
								float RandomChance = FMath::FRand();
								if (RandomChance < 0.01)
								{
									AttackerUnit->MakeHero();
								}
							}
						}
						// capture location for attacker faction
						TargetLocation->RecoverDamage(TargetLocation->MaxHealth);
						TargetLocation->FactionDataComponent->FactionData = AttackerUnit->FactionDataComponent->FactionData;
						TargetLocation->ColorTerritoryTiles();
					}
				}
			}
			if (bDidDamage)
			{
				UE_LOG(LogTemp, Display, TEXT("Dealt %s damage"), *FString::FromInt(Damage));
				AttackerUnit->UnitDataComponent->RemoveUnitActions(Cast<AFantasyMissionPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->UsedAction->ActionCost);
			}
		}
	}
}

TArray<AMapTile*> AFantasyMissionGameState::GetAdjacentTilesList(AMapTile* StartTile, int Depth)
{
	TArray<AMapTile*> ReturnMapTileList = StartTile->GetAdjacentTiles();
	if (Depth > 1)
	{
		TArray<AMapTile*> IterateMapTileList = ReturnMapTileList;
		for (int i = 1; i < Depth; i++)
		{
			TArray<AMapTile*> TempMapTileList = TArray<AMapTile*>();
			for (AMapTile* TempMapTile : IterateMapTileList)
			{
				for (AMapTile* AdjacentTile : TempMapTile->GetAdjacentTiles())
				{
					TempMapTileList.AddUnique(AdjacentTile);
					ReturnMapTileList.AddUnique(AdjacentTile);
				}
			}
			IterateMapTileList = TempMapTileList;
		}
	}
	ReturnMapTileList.Remove(StartTile);
	return ReturnMapTileList;
}

void AFantasyMissionGameState::AddUnit(AFantasyMissionCharacter* NewUnit)
{
	Units.Add(NewUnit);
}

void AFantasyMissionGameState::RemoveUnit(AFantasyMissionCharacter* UnitToRemove)
{
	Units.Remove(UnitToRemove);
}

bool AFantasyMissionGameState::CheckIfOpponentUnitInMapTile(AMapTile* MapTile, FString FactionID)
{
	for (AFantasyMissionCharacter* Unit : Units)
	{
		if (Unit->FactionDataComponent->FactionData->FactionID != FactionID && Unit->ActorSelectionComponent->CurrentMapTile == MapTile)
		{
			return true;
		}
	}
	return false;
}

void AFantasyMissionGameState::RemoveResourceFromTurnPlayer(EResourceType Resource, int Number)
{
	Players[TurnTakerList[TurnTakerIndex]]->ConsumeResource(Resource, Number);
}

void AFantasyMissionGameState::AddResourceToTurnPlayer(EResourceType Resource, int Number)
{
	Players[TurnTakerList[TurnTakerIndex]]->AddResource(Resource, Number);
}

void AFantasyMissionGameState::AddNewUnitToProductionLine(AMapTile* MapTile, EUnitType UnitType)
{
	ProductionLine.Add(MapTile, std::make_pair(UnitType, 1));
}


void AFantasyMissionGameState::RemoveUnitFromProductionLine(AMapTile* MapTile)
{
	ProductionLine.Remove(MapTile);
}

TMap<AMapTile*, EUnitType> AFantasyMissionGameState::AdvanceTurnPlayerProductionLine(TArray<ALocation*> Locations)
{
	TArray<ALocation*> TurnTakerLocations = TArray<ALocation*>();
	for (ALocation* TempLocation : Locations)
	{
		if (TempLocation->FactionDataComponent->FactionData->FactionID == GetCurrentPlayerFaction())
		{
			TurnTakerLocations.Add(TempLocation);
		}
	}
	TMap<AMapTile*, EUnitType> UnitsToSpawnList = TMap<AMapTile*, EUnitType>();
	for (ALocation* TempLocation : TurnTakerLocations)
	{
		AMapTile* TempMapTile = TempLocation->ActorSelectionComponent->CurrentMapTile;
		if (ProductionLine.Contains(TempMapTile))
		{
			std::pair<EUnitType, int>* TempProduction = ProductionLine.Find(TempMapTile);
			int TimeLeft = TempProduction->second - 1;
			if (TimeLeft == 0)
			{
				// gain experience for location
				UUnit* TempUnit = NewObject<UUnit>();
				TempUnit->SetUnitType(TempProduction->first);
				TempUnit->SetUnitValues();
				int Yield = TempUnit->EXPYield;
				bool Boost = false;
				if (CurrentWorldEvent->Type == EWorldEventType::EXPBoost)
				{
					Boost = true;
				}
				TempLocation->GainExperience(Yield, Boost);

				//remove from production line and add to spawn list
				ProductionLine.Remove(TempMapTile);
				UnitsToSpawnList.Add(TempMapTile, TempProduction->first);
			}
		}
	}
	return UnitsToSpawnList;
}

FString AFantasyMissionGameState::GetCurrentPlayerFaction()
{
	return Players[TurnTakerList[TurnTakerIndex]]->Faction->FactionID;
}

void AFantasyMissionGameState::SetPlayerFactions(TArray<FString> PlayerNames, TArray<UFaction*> PlayerFactions)
{
	TMap<FString, UPlayerData*> TempMap = TMap<FString, UPlayerData*>();
	for (int i = 0; i < PlayerNames.Num(); i++)
	{
		UPlayerData* Player = NewObject<UPlayerData>();
		Player->Name = PlayerNames[i];
		Player->Faction = PlayerFactions[i];
		TempMap.Add(PlayerNames[i], Player);
	}
	Players = TempMap;
}

void AFantasyMissionGameState::AddDefaultRelations(FString PlayerName, TArray<FString> AllPlayerNames)
{
	for (FString TempName : AllPlayerNames)
	{
		if (TempName != PlayerName)
		{
			Players[PlayerName]->Relations.Add(TempName, EPlayerRelation::Neutral);
		}
	}
}

void AFantasyMissionGameState::SetTurnTakerList(TArray<FString> TurnTakers)
{
	TurnTakerList = TurnTakers;
}

void AFantasyMissionGameState::SetTurnTakerIndex(int NewIndex)
{
	TurnTakerIndex = NewIndex;
}

void AFantasyMissionGameState::ChangeTurnTaker()
{
	if (TurnTakerIndex != TurnTakerList.Num()-1)
	{
		TurnTakerIndex++;
	}
	else
	{
		TurnTakerIndex = 0;
	}
}

void AFantasyMissionGameState::CreateTerrains()
{
	for (ETerrainType Type : TEnumRange<ETerrainType>())
	{
		UTerrain* Terrain = NewObject<UTerrain>();
		Terrain->SetTerrainType(Type);
		Terrain->SetTerrainValues();
		Terrains.Add(Terrain);
	}
}

void AFantasyMissionGameState::SetMapTiles(TArray<AMapTile*> Tiles)
{
	MapTiles = Tiles;
}

void AFantasyMissionGameState::SetResourcesToMapTiles()
{
	for (AMapTile* TempTile : MapTiles)
	{
		switch (TempTile->TerrainComponent->TerrainData->Type)
		{
		case ETerrainType::Plain:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Food);
			break;
		case ETerrainType::Hills:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Food);
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Production);
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Wood);
			break;
		case ETerrainType::Mountains:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Production);
			break;
		case ETerrainType::Swamp:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Production);
			break;
		case ETerrainType::Marsh:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Food);
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Wood);
			break;
		case ETerrainType::Glacier:
			break;
		case ETerrainType::Ocean:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Food);
			break;
		case ETerrainType::Plateau:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Production);
			break;
		case ETerrainType::Desert:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Production);
			break;
		case ETerrainType::Tundra:
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Food);
			TempTile->ResourceComponent->AddResourceOfType(EResourceType::Wood);
			break;
		}
	}
}

void AFantasyMissionGameState::SetMapTileTerrains()
{
	int ArcticTerrainIndexes[] = { 5,6 };
	int TundraTerrainIndexes[] = { 9,1,2};
	int DesertTerrainIndexes[] = { 8 };
	int VarietyTerrainIndexes[] = { 0,1,3,4,7 };
	for (AMapTile* TempTile : MapTiles)
	{
		UTerrain* RandomTerrain = NewObject<UTerrain>();
		if (ArcticLocations.Contains(TempTile->GetActorLocation()))
		{
			RandomTerrain = Terrains[ArcticTerrainIndexes[FMath::RandRange(0, 1)]];
		}
		else if (TundraLocations.Contains(TempTile->GetActorLocation()))
		{
			RandomTerrain = Terrains[TundraTerrainIndexes[FMath::RandRange(0, 2)]];
		}
		else if (DesertLocations.Contains(TempTile->GetActorLocation()))
		{
			RandomTerrain = Terrains[DesertTerrainIndexes[FMath::RandRange(0, 0)]];
		}
		else if (VarietyLocations.Contains(TempTile->GetActorLocation()))
		{
			RandomTerrain = Terrains[VarietyTerrainIndexes[FMath::RandRange(0, 4)]];
		}
		UTerrain* NewTerrain = NewObject<UTerrain>();
		NewTerrain->SetTerrainType(RandomTerrain->Type);
		NewTerrain->SetTerrainValues();
		TempTile->TerrainComponent->SetTerrainData(NewTerrain);
		int RandomNumber = FMath::RandRange(0, 200);
		if (RandomNumber == 0 && NewTerrain->Type != ETerrainType::Ocean)
		{
			TempTile->TerrainComponent->TerrainData->AddTerrainFeature(ETerrainFeature::Ruin);
		}
		TempTile->ApplyTerrainColor();
	}
}

//Calculate terrain positions
TArray<int> AFantasyMissionGameState::CalculateTerrainPositions(int Amount, int FirstTopIndex, int FirstBottomIndex)
{
	TArray<int> Indexes = TArray<int>();
	Indexes.Add(FirstTopIndex);
	Indexes.Add(FirstBottomIndex);
	int AmountOfTop = 1;
	int AmountOfBottom = 1;
	for (int i = 2; i < Amount; i++)
	{
		if (i % 2 == 0)
		{
			Indexes.Add(Indexes[0] - AmountOfTop);
			AmountOfTop++;
		}
		else
		{
			Indexes.Add(Indexes[1] + AmountOfBottom);
			AmountOfBottom++;
		}
	}
	return Indexes;
}

TArray<FVector> AFantasyMissionGameState::CalculateMapTileTransforms(FVector StartingPoint, int XAmount, int YAmount)
{
	TArray<FVector> MapTileLocations = {};
	int AmountToDistribute = XAmount - 2;

	//Tundra terrain
	int TundraAmount = AmountToDistribute / 4;
	TArray<int> TundraIndexes = CalculateTerrainPositions(TundraAmount, XAmount - 2, 1);

	//Desert terrain
	int DesertAmount = AmountToDistribute / 4;
	TArray<int> DesertIndexes = CalculateTerrainPositions(DesertAmount, XAmount / 2 - 1, XAmount / 2);


	//Calculating locations
	for (int i = 0; i < YAmount / 2; i++)
	{
		FVector tempVector = FVector(StartingPoint.X, StartingPoint.Y + 302 * i, StartingPoint.Z);
		MapTileLocations.Add(tempVector);
		ArcticLocations.Add(tempVector);
	}
	FVector SecondStartingPoint = FVector(StartingPoint.X + 87, StartingPoint.Y + 151, StartingPoint.Z);
	for (int i = 0; i < YAmount / 2; i++)
	{
		FVector tempVector = FVector(SecondStartingPoint.X, SecondStartingPoint.Y + 302 * i, SecondStartingPoint.Z);
		MapTileLocations.Add(tempVector);
		ArcticLocations.Add(tempVector);
	}
	for (int i = 1; i < XAmount; i++)
	{
		for (int j = 0; j < YAmount; j++)
		{
			FVector tempVector = FVector(MapTileLocations[j].X + 174 * i, MapTileLocations[j].Y, MapTileLocations[j].Z);
			MapTileLocations.Add(tempVector);
			if (i == XAmount-1)
			{
				ArcticLocations.Add(tempVector);
			}
			else if (TundraIndexes.Contains(i))
			{
				TundraLocations.Add(tempVector);
			}
			else if (DesertIndexes.Contains(i))
			{
				DesertLocations.Add(tempVector);
			}
			else
			{
				VarietyLocations.Add(tempVector);
			}
		}

	}
	return MapTileLocations;
}

void AFantasyMissionGameState::CreateElements()
{
	for (EActionElement Type : TEnumRange<EActionElement>())
	{
		UElement* Element = NewObject<UElement>();
		Element->SetElementType(Type);
		Element->SetElementValues();
		Elements.Add(Element);
	}
}

UElement* AFantasyMissionGameState::GetElementFromString(FString Name)
{
	UElement* NoneElement = NewObject<UElement>();
	for (UElement* Element : Elements)
	{
		if (Name == UEnum::GetValueAsString(Element->Type))
		{
			return Element;
		}
		if (Element->Type == EActionElement::None)
		{
			NoneElement = Element;
		}
	}
	return NoneElement;
}

void AFantasyMissionGameState::LoadFactionData()
{
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	Factions = TArray<UFaction*>();
	FString FilePath = FPlatformProcess::UserDir();
	FilePath += "FantasyMission/factions.json";
	if (FileManager.FileExists(*FilePath))
	{
		FString JsonString;
		FFileHelper::LoadFileToString(JsonString, *FilePath);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		FJsonSerializer::Deserialize(JsonReader, JsonObject);
		for (TSharedPtr<FJsonValue> Temp : JsonObject->GetArrayField("Factions"))
		{
			TSharedPtr<FJsonObject> FactionObject = Temp->AsObject();
			UFaction* NewFaction = NewObject<UFaction>();
			NewFaction->SetFactionData(FactionObject->GetStringField("id"), FactionObject->GetStringField("name"), CreateLinearColor(FactionObject->GetStringField("color")));
			Factions.Add(NewFaction);
		}
	}
}

FLinearColor AFantasyMissionGameState::CreateLinearColor(FString ColorString)
{
	TArray<FString> Strings = TArray<FString>();
	ColorString.ParseIntoArray(Strings, TEXT(","), false);
	float R = 0;
	float G = 0;
	float B = 0;
	float A = 1;
	if (Strings.Num() > 0)
	{
		R = FCString::Atof(*Strings[0]);
	}
	if (Strings.Num() > 1)
	{
		G = FCString::Atof(*Strings[1]);
	}
	if (Strings.Num() > 2)
	{
		B = FCString::Atof(*Strings[2]);
	}
	if (Strings.Num() > 3)
	{
		A = FCString::Atof(*Strings[3]);
	}
	FLinearColor ParsedColor = FLinearColor::FromSRGBColor(FColor(R, G, B, A));

	UE_LOG(LogTemp, Display, TEXT("%s"), *ParsedColor.ToString());
	return ParsedColor;
}