// Fill out your copyright notice in the Description page of Project Settings.


#include "Location.h"

// Sets default values
ALocation::ALocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActorSelectionComponent = CreateDefaultSubobject<UActorSelectionComponent>(TEXT("ActorSelection"));
	FactionDataComponent = CreateDefaultSubobject<UFactionDataComponent>(TEXT("FactionData"));

	LocationLevel = 1;
	Health = 20;
	MaxHealth = 20;
	Experience = 0;
	LevelUpEXP = 100;
	EXPYield = 100;
	DefenseModifier = 5;
}

int ALocation::GetEXPYield()
{
	return EXPYield * LocationLevel;
}

void ALocation::RecoverDamage(int Damage)
{
	Health += Damage;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void ALocation::TakeDamage(int Damage)
{
	Health -= Damage;
	if (Health < 0)
	{
		Health = 0;
	}
}

void ALocation::Heal(int Amount)
{
	Health += Amount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}


// Called when the game starts or when spawned
void ALocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALocation::SetTerritory()
{
	Territory.AddUnique(ActorSelectionComponent->CurrentMapTile);
	TArray<AMapTile*> AdjacentTiles = ActorSelectionComponent->CurrentMapTile->GetAdjacentTiles();
	for (AMapTile* AdjacentTile : AdjacentTiles)
	{
		Territory.AddUnique(AdjacentTile);
	}
}

void ALocation::AddTerritoryTileAtRandom()
{
	TArray<AMapTile*> PotentialTiles = TArray<AMapTile*>();
	for (AMapTile* TerritoryTile : Territory)
	{
		for (AMapTile* AdjacentTile : TerritoryTile->GetAdjacentTiles())
		{
			if (!AdjacentTile->bIsTerritory)
			{
				PotentialTiles.AddUnique(AdjacentTile);
			}
		}
	}
	if (!PotentialTiles.IsEmpty())
	{
		int RandomInt = FMath::RandRange(0, PotentialTiles.Num()-1);
		Territory.AddUnique(PotentialTiles[RandomInt]);
		ColorTerritoryTiles();
	}
}

void ALocation::ColorTerritoryTiles()
{
	//have some kind of faction material ready to go before this
	for (AMapTile* TerritoryTile : Territory)
	{
		TerritoryTile->ApplyTerritoryFunctions(FactionDataComponent->FactionData->FactionColor);
	}
}

void ALocation::GiveName(FString NewName)
{
	LocationName = NewName;
}

void ALocation::GainExperience(int Yield, bool Boost)
{
	int Multiplier = 1;
	if (Boost)
	{
		Multiplier = 2;
	}
	Experience += Yield * Multiplier;
	while (Experience >= LevelUpEXP * LocationLevel)
	{
		GainLevel();
	}
}

void ALocation::GainLevel()
{
	Experience = Experience - LevelUpEXP * LocationLevel;
	LocationLevel += 1;
	Health = Health + LocationLevel;
	MaxHealth = MaxHealth + LocationLevel;
	DefenseModifier = DefenseModifier + LocationLevel;
}

void ALocation::SetElement(EActionElement NewElement)
{
	Element = NewElement;
}