// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FantasyMission/Objects/Faction.h"
#include "FantasyMission/Objects/Resource.h"
#include "FantasyMission/Actors/Location.h"
#include "FantasyMission/Pawns/FantasyMissionCharacter.h"
#include "PlayerData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerRelation : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy")
};

UCLASS(BlueprintType)
class FANTASYMISSION_API UPlayerData : public UObject
{
	GENERATED_BODY()

public:
	UPlayerData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	UFaction* Faction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	int Money;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	TMap<EResourceType, int> Resources;

	UFUNCTION(BlueprintCallable, category = Resources)
	void AddResource(EResourceType ResourceType, int Amount);

	UFUNCTION(BlueprintCallable, category = Resources)
	void GainResources(TArray<ALocation*> Locations);

	UFUNCTION(BlueprintCallable, category = Resources)
	void LoseResources(TArray<AFantasyMissionCharacter*> Units);

	UFUNCTION(BlueprintCallable, category = Resources)
	void ConsumeResource(EResourceType Resource, int Amount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Diplomacy)
	TMap<FString, EPlayerRelation> Relations;
};
