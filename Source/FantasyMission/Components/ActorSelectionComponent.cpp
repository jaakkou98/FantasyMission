// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSelectionComponent.h"
#include "FantasyMission/Controllers/FantasyMissionPlayerController.h"

// Sets default values for this component's properties
UActorSelectionComponent::UActorSelectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UActorSelectionComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Owner = GetOwner();
	SetMesh();
}

void UActorSelectionComponent::SetMesh()
{
	TArray<UActorComponent*> TempArray;
	Owner->GetComponents(TempArray);
	for (UActorComponent* Actor : TempArray)
	{
		UE_LOG(LogTemp, Display, TEXT("'%s'"), *GetNameSafe(Actor));
		if (GetNameSafe(Actor) == "CharacterMesh0" || GetNameSafe(Actor) == "LocationMesh")
		{
			if (UMeshComponent* TempMesh = Cast<UMeshComponent>(Actor))
			{
				Mesh = TempMesh;
			}
		}
	}
}


// Called every frame
void UActorSelectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActorSelectionComponent::OnMouseOver()
{
	UE_LOG(LogTemp, Display, TEXT("Hovering Over"));
	//make the pawn outline flare up so you can tell it's beeing interacted with
	Mesh->SetOverlayMaterial(HighlightMaterial);
	UE_LOG(LogTemp, Display, TEXT("'%s'"), *Mesh->GetName());
}

void UActorSelectionComponent::OnMeshClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Possess Pawn"));

	if (AFantasyMissionPlayerController* TempController = Cast<AFantasyMissionPlayerController>(PlayerController))
	{
		TempController->SetMoveAction(false);
		//add selected object
		if (TempController->SelectedObjects.Contains(this))
		{
			TempController->RemoveSelectedObject(this);
		}
		else
		{
			TempController->AddSelectedObject(this);
		}
	}
}

void UActorSelectionComponent::OnMouseOverEnd()
{
	CheckIfSelected();
	UE_LOG(LogTemp, Display, TEXT("No Longer Over"));
}

void UActorSelectionComponent::CheckIfSelected()
{
	if (AFantasyMissionPlayerController* TempController = Cast<AFantasyMissionPlayerController>(PlayerController))
	{
		if (!TempController->SelectedObjects.Contains(this))
		{
			Mesh->SetOverlayMaterial(NULL);
		}
	}
}

void UActorSelectionComponent::SetCurrentMapTile(AMapTile* NewMapTile)
{
	CurrentMapTile = NewMapTile;
}

