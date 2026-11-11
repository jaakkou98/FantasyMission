// Copyright Epic Games, Inc. All Rights Reserved.

#include "FantasyMissionCharacter.h"
#include "FantasyMission/Controllers/FantasyMissionGameState.h"

AFantasyMissionCharacter::AFantasyMissionCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DestinationPathArray = TArray<AMapTile*>();
	MovePathIndex = 0;
	bMove = false;
	bDead = false;
	bHero = false;

	//create components
	ActorSelectionComponent = CreateDefaultSubobject<UActorSelectionComponent>(TEXT("ActorSelection"));
	FactionDataComponent = CreateDefaultSubobject<UFactionDataComponent>(TEXT("FactionData"));
	UnitDataComponent = CreateDefaultSubobject<UUnitDataComponent>(TEXT("UnitData"));
}

void AFantasyMissionCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AFantasyMissionCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	if (bMove)
	{
		AMapTile* MapTile = DestinationPathArray[MovePathIndex];
		bMove = false;
		if (MapTile->TerrainComponent->TerrainData->MovementModifier <= UnitDataComponent->ActionsAvailable && !Cast<AFantasyMissionGameState>(GetWorld()->GetGameState())->CheckIfOpponentUnitInMapTile(MapTile, FactionDataComponent->FactionData->FactionID))
		{
			UnitDataComponent->RemoveUnitActions(MapTile->TerrainComponent->TerrainData->MovementModifier);
			MoveToLocation(MapTile);
		}
		else
		{
			StopLocation = ActorSelectionComponent->CurrentMapTile->GetActorLocation();
			StopLocation = FVector(StopLocation.X, StopLocation.Y, GetActorLocation().Z);
			DestinationPathArray.Empty();
			MovePathIndex = 0;
			bFinishMoving = true;
		}
	}
	else if (DestinationPathArray.Num() > 0)
	{

		//UE_LOG(LogTemp, Display, TEXT("Distance to DestinationPathArray location for some reason is: %s"), *FString::FromInt(GetDistanceTo(DestinationPathArray[MovePathIndex])));
		if (GetDistanceTo(DestinationPathArray[MovePathIndex])<118)
		{
			if (MovePathIndex != DestinationPathArray.Num()-1)
			{
				MovePathIndex++;
				bMove = true;
			}
			else
			{
				StopLocation = DestinationPathArray[MovePathIndex]->GetActorLocation();
				StopLocation = FVector(StopLocation.X, StopLocation.Y, GetActorLocation().Z);
				DestinationPathArray.Empty();
				MovePathIndex = 0;
				bFinishMoving = true;
			}
		}
	}
	//smoothly move to the exact final location
	if(bFinishMoving)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), StopLocation, DeltaSeconds, 20));
		if (GetActorLocation() == StopLocation)
		{
			bFinishMoving = false;
		}
	}
}

void AFantasyMissionCharacter::MoveUnit(TArray<AMapTile*> NewDestinationPathArray)
{
	MovePathIndex = 0;
	DestinationPathArray = NewDestinationPathArray;
	bMove = true;
	bFinishMoving = false;
}

void AFantasyMissionCharacter::MoveToLocation(AMapTile* Destination)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Destination->GetActorLocation());
	UE_LOG(LogTemp, Display, TEXT("Moved to %s"), *Destination->GetName());
	ActorSelectionComponent->SetCurrentMapTile(Destination);
	if (ActorSelectionComponent->CurrentMapTile->TerrainComponent->TerrainData->Features.Contains(ETerrainFeature::Ruin))
	{
		Cast<AFantasyMissionGameState>(GetWorld()->GetGameState())->ClearRuin(this, Destination);
	}
}

void AFantasyMissionCharacter::ApplyFactionColor()
{
	FLinearColor FactionColor = FactionDataComponent->FactionData->FactionColor;
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	NewMaterial->SetVectorParameterValue(FName(TEXT("Color")), FactionColor);
	GetMesh()->SetMaterial(0, NewMaterial);
}

void AFantasyMissionCharacter::MakeHero()
{
	HeroData = NewObject<UHeroData>();
	bHero = true;
	UnitDataComponent->UnitData->ActionList.Add(EUnitAction::HeroAttack);
}