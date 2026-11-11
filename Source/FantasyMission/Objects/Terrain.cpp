// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain.h"

UTerrain::UTerrain()
{
	Features = TArray<ETerrainFeature>();
}

void UTerrain::SetTerrainType(ETerrainType NewType)
{
	Type = NewType;
}

void UTerrain::AddTerrainFeature(ETerrainFeature NewFeature)
{
	Features.Add(NewFeature);
}

void UTerrain::SetTerrainValues()
{
	switch (Type)
	{
	case ETerrainType::Plain:
		Color = FLinearColor(FColor(21, 161, 47, 1));
		MovementModifier = 1;
		break;
	case ETerrainType::Hills:
		Color = FLinearColor(FColor(121, 134, 145, 1));
		MovementModifier = 2;
		break;
	case ETerrainType::Mountains:
		Color = FLinearColor(FColor(143, 114, 76, 1));
		MovementModifier = 4;
		break;
	case ETerrainType::Swamp:
		Color = FLinearColor(FColor(170, 255, 150, 1));
		MovementModifier = 3;
		break;
	case ETerrainType::Marsh:
		Color = FLinearColor(FColor(105, 135, 97, 1));
		MovementModifier = 2;
		break;
	case ETerrainType::Glacier:
		Color = FLinearColor(FColor(0, 255, 229, 1));
		MovementModifier = 3;
		break;
	case ETerrainType::Ocean:
		Color = FLinearColor(FColor(0, 26, 255, 1));
		MovementModifier = -1;
		break;
	case ETerrainType::Plateau:
		Color = FLinearColor(FColor(204, 202, 61, 1));
		MovementModifier = 2;
		break;
	case ETerrainType::Desert:
		Color = FLinearColor(FColor(171, 144, 9, 1));
		MovementModifier = 2;
		break;
	case ETerrainType::Tundra:
		Color = FLinearColor(FColor(44, 156, 100, 1));
		MovementModifier = 1;
		break;
	}
}