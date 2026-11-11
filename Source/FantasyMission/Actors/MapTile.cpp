// Fill out your copyright notice in the Description page of Project Settings.


#include "MapTile.h"

// Sets default values
AMapTile::AMapTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* NewSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = NewSceneComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapTile"));
	StaticMesh->SetupAttachment(RootComponent);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Models/hexagon"));
	StaticMesh->SetStaticMesh(MeshObj.Object);

	//tile color overlay creation
	TileColorOverlay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileColorOverlay"));
	TileColorOverlay->SetupAttachment(RootComponent);
	TileColorOverlay->SetStaticMesh(MeshObj.Object);
	TileColorOverlay->SetMaterial(0, TileColorMaterial);
	TileColorOverlay->AddLocalOffset(FVector(0.0f, 0.0f, 10.0f), false, nullptr, ETeleportType::None);

	//components
	TerrainComponent = CreateDefaultSubobject<UTerrainComponent>(TEXT("Terrain"));
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("Resources"));

	bIsTerritory = false;
}

//Basics
// Called when the game starts or when spawned
void AMapTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Adjacent tiles
void AMapTile::AddAdjacentTile(AMapTile* AdjacentTile)
{
	if (!AdjacentTiles.Contains(AdjacentTile))
	{
		AdjacentTiles.Add(AdjacentTile);
	}
}

TArray<AMapTile*> AMapTile::GetAdjacentTiles()
{
	return AdjacentTiles;
}

//Color
void AMapTile::ApplyTerrainColor()
{
	FLinearColor TerrainColor = TerrainComponent->TerrainData->Color;
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this);
	NewMaterial->SetVectorParameterValue(FName(TEXT("Color")), TerrainColor);
	StaticMesh->SetMaterial(0, NewMaterial);
}

void AMapTile::ApplyTerritoryFunctions(FLinearColor NewColor)
{
	TerritoryColor = NewColor;
	bIsTerritory = true;
	ChangeTileOverlayColor(NewColor);
	ToggleColorTileVisibility(true);
}

void AMapTile::ToggleColorTileVisibility(bool bVisible)
{
	TileColorOverlay->SetVisibility(bVisible);
}

void AMapTile::ChangeTileOverlayColor(FLinearColor NewColor)
{
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(TileColorMaterial, this);
	NewMaterial->SetVectorParameterValue(FName(TEXT("Color")), NewColor);
	TileColorOverlay->SetMaterial(0, NewMaterial);
}

void AMapTile::HighlightTile(FLinearColor NewColor)
{
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(HighlightMaterial, this);
	NewMaterial->SetVectorParameterValue(FName(TEXT("Color")), NewColor);
	TileColorOverlay->SetMaterial(0, NewMaterial);
	ToggleColorTileVisibility(true);
}

void AMapTile::UnhighlightTile()
{
	TileColorOverlay->SetMaterial(0, TileColorMaterial);
	if (bIsTerritory)
	{
		ChangeTileOverlayColor(TerritoryColor);
	}
	else
	{
		ToggleColorTileVisibility(false);
	}
}


//Navigation
int AMapTile::GetMovementModifier()
{
	return TerrainComponent->TerrainData->MovementModifier;
}

float AMapTile::CalculateDistance(const FVector A, const FVector B)
{
	return (float)(FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z));
}

AMapTile* AMapTile::GetClosestAdjacentTile(AMapTile* Destination, TArray<AMapTile*> Exclude)
{
	AMapTile* ClosestTile = NULL;
	float ClosestDistance = 90000;
	for (AMapTile* MapTile : AdjacentTiles)
	{
		if (!Exclude.Contains(MapTile) && MapTile->GetMovementModifier() > 0)
		{
			float Distance = CalculateDistance(MapTile->GetActorLocation(), Destination->GetActorLocation());

			UE_LOG(LogTemp, Display, TEXT("Calculated Distance: %s"), *FString::FromInt(Distance));
			Distance *= 1.0f+MapTile->GetMovementModifier()/10.0f;
			if (ClosestDistance > Distance)
			{
				ClosestDistance = Distance;
				ClosestTile = MapTile;
			}
		}
	}
	return ClosestTile;
}

TArray<AMapTile*> AMapTile::FindPath(AMapTile* Destination)
{
	TArray<AMapTile*> PathA = TArray<AMapTile*>();
	TArray<AMapTile*> PathB = TArray<AMapTile*>();
	TArray<AMapTile*> PathC = TArray<AMapTile*>();
	TArray<AMapTile*> PathD = TArray<AMapTile*>();
	TArray<AMapTile*> PathE = TArray<AMapTile*>();
	TArray<AMapTile*> PathF = TArray<AMapTile*>();
	TArray<AMapTile*> ExcludeList = TArray<AMapTile*>();
	ExcludeList.Add(this);

	//if adjacent tile contains destination, just return destination
	if (AdjacentTiles.Contains(Destination))
	{
		PathA.Add(Destination);
		return PathA;
	}

	//add first two paths because all tiles have at least two adjacent IF they are available
	if (AdjacentTiles[0]->GetMovementModifier() > 0)
	{
		PathA.Add(AdjacentTiles[0]);
	}
	if (AdjacentTiles[1]->GetMovementModifier() > 0)
	{
		PathB.Add(AdjacentTiles[1]);
	}

	if (AdjacentTiles.Num() >= 3)
	{
		if (AdjacentTiles[2]->GetMovementModifier() > 0)
		{
			PathC.Add(AdjacentTiles[2]);
		}
	}
	if (AdjacentTiles.Num() >= 4)
	{
		if (AdjacentTiles[3]->GetMovementModifier() > 0)
		{
			PathD.Add(AdjacentTiles[3]);
		}
	}
	if (AdjacentTiles.Num() >= 5)
	{
		if (AdjacentTiles[4]->GetMovementModifier() > 0)
		{
			PathE.Add(AdjacentTiles[4]);
		}
	}
	if (AdjacentTiles.Num() >= 6)
	{
		if (AdjacentTiles[5]->GetMovementModifier() > 0)
		{
			PathF.Add(AdjacentTiles[5]);
		}
	}

	//add adjacent tiles to exclude list
	for (AMapTile* MapTile : AdjacentTiles)
	{
		ExcludeList.AddUnique(MapTile);
	}

	bool FoundDestinationArray[6] = { false,false,false,false,false,false };

	//search for the paths

	if (PathA.Num() > 0)
	{
		std::pair PairA = SearchDestination(Destination, ExcludeList, PathA);
		FoundDestinationArray[0] = PairA.first;
		PathA = PairA.second;
	}
	if (PathB.Num() > 0)
	{
		std::pair PairB = SearchDestination(Destination, ExcludeList, PathB);
		FoundDestinationArray[1] = PairB.first;
		PathB = PairB.second;
	}
	if (PathC.Num() > 0)
	{
		std::pair PairC = SearchDestination(Destination, ExcludeList, PathC);
		FoundDestinationArray[2] = PairC.first;
		PathC = PairC.second;
	}
	if (PathD.Num() > 0)
	{
		std::pair PairD = SearchDestination(Destination, ExcludeList, PathD);
		FoundDestinationArray[3] = PairD.first;
		PathD = PairD.second;
	}
	if (PathE.Num() > 0)
	{
		std::pair PairE = SearchDestination(Destination, ExcludeList, PathE);
		FoundDestinationArray[4] = PairE.first;
		PathE = PairE.second;
	}
	if (PathF.Num() > 0)
	{
		std::pair PairF = SearchDestination(Destination, ExcludeList, PathF);
		FoundDestinationArray[5] = PairF.first;
		PathF = PairF.second;
	}

	float PathCosts[6] = {0,0,0,0,0,0};

	//check movement costs
	if (FoundDestinationArray[0])
	{
		for (AMapTile* MapTile : PathA)
		{
			PathCosts[0] += MapTile->GetMovementModifier();
		}
	}
	if (FoundDestinationArray[1])
	{
		for (AMapTile* MapTile : PathB)
		{
			PathCosts[1] += MapTile->GetMovementModifier();
		}
	}
	if (FoundDestinationArray[2])
	{
		for (AMapTile* MapTile : PathC)
		{
			PathCosts[2] += MapTile->GetMovementModifier();
		}
	}
	if (FoundDestinationArray[3])
	{
		for (AMapTile* MapTile : PathD)
		{
			PathCosts[3] += MapTile->GetMovementModifier();
		}
	}
	if (FoundDestinationArray[4])
	{
		for (AMapTile* MapTile : PathE)
		{
			PathCosts[4] += MapTile->GetMovementModifier();
		}
	}
	if (FoundDestinationArray[5])
	{
		for (AMapTile* MapTile : PathF)
		{
			PathCosts[5] += MapTile->GetMovementModifier();
		}
	}
	int Index = -1;
	int TempSmallest = 5000;
	for (int i = 0; i < 6; i++)
	{
		if (PathCosts[i] != 0 && TempSmallest > PathCosts[i])
		{
			TempSmallest = PathCosts[i];
			Index = i;
		}
	}

	TArray<AMapTile*> FastestPath = TArray<AMapTile*>();


	UE_LOG(LogTemp, Display, TEXT("Index %s"), *FString::FromInt(Index));
	//returns
	switch (Index)
	{
		case 0:
			UE_LOG(LogTemp, Display, TEXT("PathA was fastest)"));
			FastestPath = PathA;
			break;
		case 1:
			UE_LOG(LogTemp, Display, TEXT("PathB was fastest)"));
			FastestPath = PathB;
			break;
		case 2:
			UE_LOG(LogTemp, Display, TEXT("PathC was fastest)"));
			FastestPath = PathC;
			break;
		case 3:
			UE_LOG(LogTemp, Display, TEXT("PathD was fastest)"));
			FastestPath = PathD;
			break;
		case 4:
			UE_LOG(LogTemp, Display, TEXT("PathE was fastest)"));
			FastestPath = PathE;
			break;
		case 5:
			UE_LOG(LogTemp, Display, TEXT("PathF was fastest)"));
			FastestPath = PathF;
			break;
		default: //if none found it
			UE_LOG(LogTemp, Display, TEXT("DID NOT FIND DESTINATION"));
			break;
	}
	return FastestPath;
}

std::pair<bool, TArray<AMapTile*>> AMapTile::SearchDestination(AMapTile* Destination, TArray<AMapTile*> Exclude, TArray<AMapTile*> Path)
{
	bool bSearchingPath = true;
	bool bFoundDestination = false;
	TArray<AMapTile*> ExcludeList = Exclude;

	int CurrentIndex = 0;
	int Tries = 5;
	while (bSearchingPath)
	{
		bool LostTry = false;
		AMapTile* TempTile = Path[CurrentIndex]->GetClosestAdjacentTile(Destination, ExcludeList);
		if (TempTile != NULL)
		{
			Path.Add(TempTile);
			ExcludeList.AddUnique(TempTile);
			if (Path.Contains(Destination))
			{
				bSearchingPath = false;
				bFoundDestination = true;

				UE_LOG(LogTemp, Display, TEXT("THIS MANY TRIES LEFT: %s"), *FString::FromInt(Tries));
			}
			else
			{
				bSearchingPath = true;
				bFoundDestination = false;
			}
		}
		else
		{
			LostTry = true;
			Tries--;
			if (Tries == 0)
			{
				bSearchingPath = false;
				bFoundDestination = false;
			}
			if (CurrentIndex > 0)
			{
				AMapTile* RemoveThis = Path[Path.Num() - 1];
				for (AMapTile* Remove : RemoveThis->AdjacentTiles)
				{
					if (!Path.Contains(Remove))
					{
						ExcludeList.Remove(Remove);
					}
				}
				CurrentIndex -= 1;
				Path.RemoveAt(Path.Num() - 1);
			}
		}
		if (!LostTry)
		{
			for (AMapTile* MapTile : Path[CurrentIndex]->AdjacentTiles)
			{
				ExcludeList.AddUnique(MapTile);
			}
			CurrentIndex++;
		}
	}
	std::pair<bool, TArray<AMapTile*>> Pair = std::make_pair(bFoundDestination, Path);
	return Pair;
}
