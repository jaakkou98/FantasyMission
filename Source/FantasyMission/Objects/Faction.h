// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Element.h"
#include "Faction.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class FANTASYMISSION_API UFaction : public UObject
{
	GENERATED_BODY()

public:
	UFaction();
public:

	UFUNCTION(BlueprintCallable, category = Factions)
	void SetFactionData(FString ID, FString Name, FLinearColor Color);

	UFUNCTION(BlueprintCallable, category = Factions)
	void SetFactionElements(TMap<EActionElement, float> Elements);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FactionID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FactionName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor FactionColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EActionElement, float> FactionElements;
};

	
