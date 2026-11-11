// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Element.h"
#include "Unit.h"
#include "WorldEvent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWorldEventType : uint8
{
	ElementBoost UMETA(DisplayName = "Element Boost"),
	EXPBoost UMETA(DisplayName = "Experience Boost"),
	UnitBoost UMETA(DisplayName = "Unit Boost"),
	NoBoost UMETA(DisplayName = "No Boost"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EWorldEventType, EWorldEventType::Count);

UCLASS()
class FANTASYMISSION_API UWorldEvent : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WorldEvents)
	int Turns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WorldEvents)
	EWorldEventType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WorldEvents)
	EUnitType UnitType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WorldEvents)
	EActionElement ElementType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WorldEvents)
	FString Description;

	UFUNCTION(BlueprintCallable, category = WorldEvents)
	void SetEventType(EWorldEventType NewType);

	UFUNCTION(BlueprintCallable, category = WorldEvents)
	void SetEventValues(UWorldEvent* CurrentEvent);

private:
	void SetElement(UWorldEvent* CurrentEvent);
	void SetUnit(UWorldEvent* CurrentEvent);
	
};
