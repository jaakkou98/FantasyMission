// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldEvent.h"
#include "FantasyMission/Controllers/FantasyMissionGameState.h"

void UWorldEvent::SetEventType(EWorldEventType NewType)
{
	Type = NewType;
}

void UWorldEvent::SetEventValues(UWorldEvent* CurrentEvent)
{
	switch (Type)
	{
	case EWorldEventType::ElementBoost:
		SetElement(CurrentEvent);
		UnitType = EUnitType::Count;
		Turns = FMath::RandRange(1, 10);
		break;
	case EWorldEventType::UnitBoost:
		SetUnit(CurrentEvent);
		ElementType = EActionElement::Count;
		Turns = FMath::RandRange(1, 10);
		break;
	case EWorldEventType::EXPBoost:
		Turns = FMath::RandRange(1, 10);
		Description = "Increased Experience Gain\n\nThe World is experiencing an experience boost. All Units and Cities gain 2x Experience.";
		break;
	case EWorldEventType::NoBoost:
		Turns = FMath::RandRange(1, 10);
		Description = "Normal Times\n\nNothing of note is happening.";
		break;
	}
}


void UWorldEvent::SetElement(UWorldEvent* CurrentEvent)
{
	TArray<EActionElement> PossibleElements = TArray<EActionElement>();
	for (EActionElement TempType : TEnumRange<EActionElement>())
	{
		if (TempType != EActionElement::None && TempType != EActionElement::Neutral)
		{
			if (CurrentEvent->Type == EWorldEventType::ElementBoost)
			{
				if (TempType != CurrentEvent->ElementType)
				{
					PossibleElements.Add(TempType);
				}
			}
			else
			{
				PossibleElements.Add(TempType);
			}
		}
	}
	ElementType = PossibleElements[FMath::RandRange(0, PossibleElements.Num() - 1)];
	FString ElementString = UEnum::GetValueAsName(ElementType).ToString();
	TArray<FString> StringArray = TArray<FString>();
	ElementString.ParseIntoArray(StringArray, TEXT(":"), true);
	FString NewString = StringArray[1];
	Description = NewString + " Boost\n\nThe World is experiencing a boost to " + NewString + " Magic. All " + NewString + " Attacks deal 2x more damage.";
}

void UWorldEvent::SetUnit(UWorldEvent* CurrentEvent)
{
	TArray<EUnitType> PossibleUnits = TArray<EUnitType>();
	for (EUnitType TempType : TEnumRange<EUnitType>())
	{
		if (TempType != EUnitType::Settler)
		{
			if (CurrentEvent->Type == EWorldEventType::UnitBoost)
			{
				if (TempType != CurrentEvent->UnitType)
				{
					PossibleUnits.Add(TempType);
				}
			}
			else
			{
				PossibleUnits.Add(TempType);
			}
		}
	}
	UnitType = PossibleUnits[FMath::RandRange(0, PossibleUnits.Num() - 1)];
	FString UnitString = UEnum::GetValueAsName(UnitType).ToString();
	TArray<FString> StringArray = TArray<FString>();
	UnitString.ParseIntoArray(StringArray, TEXT(":"), true);
	FString NewString = StringArray[1];
	Description = NewString + " Boost\n\nThe World is experiencing a boost to " + NewString + " Units. All " + NewString + " Units deal 2x more damage.";
}