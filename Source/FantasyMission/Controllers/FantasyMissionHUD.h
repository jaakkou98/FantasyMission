// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FantasyMission/Components/ActorSelectionComponent.h"
#include "FantasyMissionHUD.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYMISSION_API AFantasyMissionHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UActorSelectionComponent*> SelectedActors;

	UFUNCTION(BlueprintCallable, category = UI)
	void SetHUDWidget(UUserWidget* NewWidget);

	UFUNCTION(BlueprintCallable, category = UI)
	void SetActorsSelected(TArray<UActorSelectionComponent*> NewlySelectedActors);

	UFUNCTION(BlueprintCallable, category = UI)
	void SetResetSelectionUI(bool NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bResetSelection;

};
