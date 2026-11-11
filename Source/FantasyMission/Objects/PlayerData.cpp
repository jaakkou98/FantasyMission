// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"

UPlayerData::UPlayerData()
{
	Money = 5;
	Relations = TMap<FString, EPlayerRelation>();
}

void UPlayerData::ConsumeResource(EResourceType Resource, int Amount)
{
	Resources[Resource] -= Amount;
	if (Resources[Resource] <= 0)
	{
		Resources.Remove(Resource);
	}
}

void UPlayerData::AddResource(EResourceType Resource, int Amount)
{
	if (Resources.Contains(Resource))
	{
		Resources[Resource] += Amount;
	}
	else
	{
		Resources.Add(Resource, Amount);
	}
}

void UPlayerData::GainResources(TArray<ALocation*> Locations)
{
	Resources.Empty();
	for (ALocation* TempLocation : Locations)
	{
		for (AMapTile* TempTerritory : TempLocation->Territory)
		{
			for (UResource* TempResource : TempTerritory->ResourceComponent->Resources)
			{
				AddResource(TempResource->Type, TempResource->Amount);
				Money += TempResource->MoneyValue*TempResource->Amount;

				UE_LOG(LogTemp, Display, TEXT("MADE %s MONEY"), *FString::FromInt(TempResource->MoneyValue * TempResource->Amount));
			}
		}
	}
}

void UPlayerData::LoseResources(TArray<AFantasyMissionCharacter*> Units)
{
	for (AFantasyMissionCharacter* TempUnit : Units)
	{
		Money -= TempUnit->UnitDataComponent->UnitData->MaintenanceCost;
	}
}