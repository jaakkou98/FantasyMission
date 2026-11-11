// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyMissionHUD.h"

void AFantasyMissionHUD::SetHUDWidget(UUserWidget* NewWidget)
{
	HUDWidget = NewWidget;
}

void AFantasyMissionHUD::SetActorsSelected(TArray<UActorSelectionComponent*> NewlySelectedActors)
{
	SelectedActors = NewlySelectedActors;
}

void AFantasyMissionHUD::SetResetSelectionUI(bool NewValue)
{
	bResetSelection = NewValue;
}