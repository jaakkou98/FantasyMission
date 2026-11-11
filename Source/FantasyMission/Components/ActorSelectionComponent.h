// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "FantasyMission/Actors/MapTile.h"
#include <Kismet/GameplayStatics.h>
#include "ActorSelectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYMISSION_API UActorSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorSelectionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, category = MouseFunctions)
	void OnMouseOver();

	UFUNCTION(BlueprintCallable, category = MouseFunctions)
	void OnMouseOverEnd();

	UFUNCTION(BlueprintCallable, category = MouseFunctions)
	void OnMeshClicked();

	//Highlight material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials, meta = (AllowPrivateAccess = "true"))
	class UMeshComponent* Mesh;

	APlayerController* PlayerController;

	void CheckIfSelected();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MapTiles, meta = (AllowPrivateAccess = "true"))
	AMapTile* CurrentMapTile;

	UFUNCTION(BlueprintCallable, category = MapTiles)
	void SetCurrentMapTile(AMapTile* NewMapTile);
		
private:
	AActor* Owner;

	void SetMesh();
};
