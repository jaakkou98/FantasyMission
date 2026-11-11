// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Terrain.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	Plain UMETA(DisplayName = "Plain"),
	Hills UMETA(DisplayName = "Hills"),
	Mountains UMETA(DisplayName = "Mountains"),
	Swamp UMETA(DisplayName = "Swamp"),
	Marsh UMETA(DisplayName = "Marsh"),
	Glacier UMETA(DisplayName = "Glacier"),
	Ocean UMETA(DisplayName = "Ocean"),
	Plateau UMETA(DisplayName = "Plateau"),
	Desert UMETA(DisplayName = "Desert"),
	Tundra UMETA(DisplayName = "Tundra"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETerrainType, ETerrainType::Count);

UENUM(BlueprintType)
enum class ETerrainFeature : uint8
{
	Beach UMETA(DisplayName = "Beach"),
	Forest UMETA(DisplayName = "Forest"),
	River UMETA(DisplayName = "River"),
	Road UMETA(DisplayName = "Road"),
	Farm UMETA(DisplayName = "Farm"),
	Buildings UMETA(DisplayName = "Buildings"),
	Valley UMETA(DisplayName = "Valley"),
	Canyon UMETA(DisplayName = "Canyon"),
	Oasis UMETA(DisplayName = "Oasis"),
	Ruin UMETA(DisplayName = "Ruin")
};

UCLASS(BlueprintType)
class FANTASYMISSION_API UTerrain : public UObject
{
	GENERATED_BODY()

public:
	UTerrain();
	int MovementModifier;
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Terrain)
	ETerrainType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Terrain)
	TArray<ETerrainFeature> Features;

	UFUNCTION(BlueprintCallable, category = Terrain)
	void SetTerrainValues();

	UFUNCTION(BlueprintCallable, category = Terrain)
	void SetTerrainType(ETerrainType NewType);

	UFUNCTION(BlueprintCallable, category = Terrain)
	void AddTerrainFeature(ETerrainFeature NewFeature);
};
