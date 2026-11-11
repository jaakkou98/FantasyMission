// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FantasyMission/Components/ActorSelectionComponent.h"
#include "FantasyMission/Components/FactionDataComponent.h"
#include "Location.generated.h"

UCLASS()
class FANTASYMISSION_API ALocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, category = Location)
	void RecoverDamage(int Damage);

	UFUNCTION(BlueprintCallable, category = Location)
	void TakeDamage(int Damage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UActorSelectionComponent* ActorSelectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFactionDataComponent* FactionDataComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AMapTile*> Territory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LocationName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Locations)
	EActionElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LocationLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Locations)
	int EXPYield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Locations)
	int Experience;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Locations)
	int LevelUpEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DefenseModifier;

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void AddTerritoryTileAtRandom();

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void SetTerritory();

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void ColorTerritoryTiles();

	UFUNCTION(BlueprintCallable, category = Locations)
	void GiveName(FString NewName);

	UFUNCTION(BlueprintCallable, category = Locations)
	void GainExperience(int Yield, bool Boost);

	UFUNCTION(BlueprintCallable, category = Locations)
	int GetEXPYield();

	UFUNCTION(BlueprintCallable, category = Locations)
	void Heal(int Amount);

	UFUNCTION(BlueprintCallable, category = Locations)
	void SetElement(EActionElement NewElement);

private:
	void GainLevel();
};
