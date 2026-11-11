// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"

void UPowerUp::SetPowerUpType(EPowerUpType NewType)
{
	Type = NewType;
}

void UPowerUp::SetPowerUpValues()
{
	switch (Type)
	{
	case EPowerUpType::Action:
		SetAction();
		break;
	case EPowerUpType::StatBoost:
		SetStatBoost();
		break;
	}
}


void UPowerUp::SetAction()
{
	TArray<EUnitAction> PossibleActions = TArray<EUnitAction>();
	for (EUnitAction TempType : TEnumRange<EUnitAction>())
	{
		if (TempType != EUnitAction::Cancel && TempType != EUnitAction::Move && TempType != EUnitAction::Produce && TempType != EUnitAction::HeroAttack && TempType != EUnitAction::Attack)
		{
			PossibleActions.Add(TempType);
		}
	}
	Action = PossibleActions[FMath::RandRange(0, PossibleActions.Num()-1)];
}

void UPowerUp::SetStatBoost()
{
	TArray<FString> PossibleStats = TArray<FString>();
	PossibleStats.Add("Actions");
	PossibleStats.Add("Attack");
	PossibleStats.Add("Defense");
	PossibleStats.Add("Health");
	StatBoost = PossibleStats[FMath::RandRange(0, PossibleStats.Num() - 1)];
}