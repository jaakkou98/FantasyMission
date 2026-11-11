// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FantasyMission/Components/TerrainComponent.h"
#include "FantasyMission/Components/ResourceComponent.h"
#include "MapTile.generated.h"

UCLASS()
class FANTASYMISSION_API AMapTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapTile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* TileColorOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Terrain)
	UTerrainComponent* TerrainComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	UResourceComponent* ResourceComponent;

	TArray<AMapTile*> AdjacentTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* TileColorMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Adjacent tiles
	UFUNCTION(BlueprintCallable, category = MapTiles)
	void AddAdjacentTile(AMapTile* AdjacentTile);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	TArray<AMapTile*> GetAdjacentTiles();

	//Navigation
	UFUNCTION(BlueprintCallable, category = MapTiles)
	TArray<AMapTile*> FindPath(AMapTile* Destination);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	int GetMovementModifier();

	//Colors
	UFUNCTION(BlueprintCallable, category = MapTiles)
	void ChangeTileOverlayColor(FLinearColor NewColor);

	void ToggleColorTileVisibility(bool bVisible);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void ApplyTerrainColor();

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void HighlightTile(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void UnhighlightTile();

	//Highlight material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* HighlightMaterial;

	void ApplyTerritoryFunctions(FLinearColor NewColor);

	bool bIsTerritory;
private:
	//Navigation
	float CalculateDistance(const FVector A, const FVector B);
	AMapTile* GetClosestAdjacentTile(AMapTile* Destination, TArray<AMapTile*> Exclude);
	std::pair<bool, TArray<AMapTile*>> SearchDestination(AMapTile* Destination, TArray<AMapTile*> Exclude, TArray<AMapTile*> Path);
	FLinearColor TerritoryColor;
};
