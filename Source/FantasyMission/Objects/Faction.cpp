// Fill out your copyright notice in the Description page of Project Settings.


#include "Faction.h"

UFaction::UFaction()
{
	FactionID = TEXT("");
	FactionName = TEXT("");
	FactionColor = FLinearColor();
}

void UFaction::SetFactionData(FString ID, FString Name, FLinearColor Color)
{
	FactionID = ID;
	FactionName = Name;
	FactionColor = Color;
}

void UFaction::SetFactionElements(TMap<EActionElement, float> Elements)
{
	FactionElements = Elements;
}