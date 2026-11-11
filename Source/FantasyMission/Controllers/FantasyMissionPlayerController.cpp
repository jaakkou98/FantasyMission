// Copyright Epic Games, Inc. All Rights Reserved.

#include "FantasyMissionPlayerController.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFantasyMissionPlayerController::AFantasyMissionPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	UnitCachedDestination = FVector::ZeroVector;
	CameraCachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	MinZoomLength = 10.f;
	MaxZoomLength = 5000.0f;
	ZoomSpeed = 50.0f;
	SelectedObjects = {};
	bIsSelectingMultiple = false;
	bAllowMovement = false;
	bMoveActionSelected = false;
	bAttackActionSelected = false;
}

bool AFantasyMissionPlayerController::CanSettle()
{
	bool bCanSettle = true;
	if (SelectedObjects[0]->CurrentMapTile->bIsTerritory)
	{
		bCanSettle = false;
	}
	else
	{
		for (AMapTile* TempTile : SelectedObjects[0]->CurrentMapTile->AdjacentTiles)
		{
			if (TempTile->bIsTerritory)
			{
				bCanSettle = false;
			}
		}
	}
	return bCanSettle;
}

void AFantasyMissionPlayerController::SetUsedAction(UAction* NewAction)
{
	UsedAction = NewAction;
	if (UsedAction->Element == EActionElement::None)
	{
		if (AFantasyMissionCharacter* TempCharacter = Cast<AFantasyMissionCharacter>(SelectedObjects[0]->GetOwner()))
		{
			UsedAction->SetActionElement(TempCharacter->UnitDataComponent->Element);
		}
	}
}

void AFantasyMissionPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	StopMovement();
}

void AFantasyMissionPlayerController::SetMoveAction(bool NewValue)
{
	bMoveActionSelected = NewValue;
}

void AFantasyMissionPlayerController::SetAttackAction(bool NewValue)
{
	bAttackActionSelected = NewValue;
}

FVector AFantasyMissionPlayerController::GetSelectedObjectLocation()
{
	if (SelectedObjects.Num() > 0)
	{
		return SelectedObjects[0]->GetOwner()->GetActorLocation();
	}
	else
	{
		return FVector();
	}
}

void AFantasyMissionPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFantasyMissionPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFantasyMissionPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFantasyMissionPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnCameraMovementTriggered);
		EnhancedInputComponent->BindAction(CameraMovementAction, ETriggerEvent::Completed, this, &AFantasyMissionPlayerController::OnCameraMovementReleased);
		EnhancedInputComponent->BindAction(CameraMovementAction, ETriggerEvent::Canceled, this, &AFantasyMissionPlayerController::OnCameraMovementReleased);

		// Setup selection inputs
		EnhancedInputComponent->BindAction(SelectingMultipleAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnSelectingMultipleTriggered);
		EnhancedInputComponent->BindAction(SelectingMultipleAction, ETriggerEvent::Completed, this, &AFantasyMissionPlayerController::OnSelectingMultipleReleased);
		EnhancedInputComponent->BindAction(SelectingMultipleAction, ETriggerEvent::Canceled, this, &AFantasyMissionPlayerController::OnSelectingMultipleReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFantasyMissionPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFantasyMissionPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFantasyMissionPlayerController::OnTouchReleased);

		// Setup keyboard input event
		EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnInputMovement);

		// Setup mouse zoom input event
		EnhancedInputComponent->BindAction(ZoomInputAction, ETriggerEvent::Triggered, this, &AFantasyMissionPlayerController::OnZoomTriggered);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFantasyMissionPlayerController::OnInputStarted()
{
	StopMovement();
}

void AFantasyMissionPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	//FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}
	// If we hit a surface, check what type of actor was hit to determine if location should be cached.
	if (bHitSuccessful)
	{
		if (Cast<AMapTile>(Hit.GetActor()) && Cast<AMapTile>(Hit.GetActor())->GetMovementModifier() > 0)
		{
			DestinationMapTile = Cast<AMapTile>(Hit.GetActor());
			UnitCachedDestination = DestinationMapTile->GetActorLocation();
			UE_LOG(LogTemp, Display, TEXT("%s"), *Hit.GetActor()->GetName());
			bAllowMovement = true;
		}
		else
		{
			bAllowMovement = false;
		}
	}
	else
	{
		bAllowMovement = false;
	}
}

// Triggered every frame when the input is held down
void AFantasyMissionPlayerController::OnCameraMovementTriggered()
{
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CameraCachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CameraCachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AFantasyMissionPlayerController::OnSetDestinationReleased()
{
	if(SelectedObjects.Num() > 0 && bAllowMovement && bMoveActionSelected)
	{
		SetMoveAction(false);
		for (UActorSelectionComponent* SelectedObject : SelectedObjects)
		{
			if (AFantasyMissionCharacter* TempCharacter = Cast<AFantasyMissionCharacter>(SelectedObject->GetOwner()))
			{
				//only move unit if faction ID matches the player ID
				if (TempCharacter->FactionDataComponent->FactionData->FactionID == Cast<AFantasyMissionGameState>(UGameplayStatics::GetGameState(GetWorld()))->GetCurrentPlayerFaction())
				{
					//spawn some particles
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, UnitCachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

					TArray<AMapTile*> DestinationPathArray = SelectedObject->CurrentMapTile->FindPath(DestinationMapTile);
					if (DestinationPathArray.Num() == 0)
					{
						//UAIBlueprintHelperLibrary::SimpleMoveToLocation(TempCharacter->Controller, UnitCachedDestination);
						UE_LOG(LogTemp, Display, TEXT("DestinationPathArray is NULL"));
					}
					else if (DestinationMapTile != SelectedObject->CurrentMapTile)
					{
						TempCharacter->MoveUnit(DestinationPathArray);
					}
				}
			}
		}
	}
}

void AFantasyMissionPlayerController::UpdateUnitActionUI()
{
	Cast<AFantasyMissionHUD>(GetHUD())->SetResetSelectionUI(true);
}

void AFantasyMissionPlayerController::OnCameraMovementReleased()
{
}

// Triggered every frame when the input is held down
void AFantasyMissionPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnCameraMovementTriggered();
}

void AFantasyMissionPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AFantasyMissionPlayerController::OnInputMovement(const FInputActionValue& Value)
{
	StopMovement();
	FVector2D moveVector = Value.Get<FVector2D>();
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector::ForwardVector, moveVector.Y);
	ControlledPawn->AddMovementInput(FVector::RightVector, moveVector.X);
}

void AFantasyMissionPlayerController::OnZoomTriggered(const FInputActionValue& Value)
{
	float zoomFloat = Value.Get<float>();
	TArray<USpringArmComponent*> array;
	GetPawn()->GetComponents<USpringArmComponent>(array);
	float NewTargetArmLength = array[0]->TargetArmLength + zoomFloat * ZoomSpeed * -1;
	array[0]->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomLength, MaxZoomLength);
}

void AFantasyMissionPlayerController::AddSelectedObject(UActorSelectionComponent* Value)
{
	if (bAttackActionSelected)
	{
		AttackTarget = Value;
		UE_LOG(LogTemp, Display, TEXT("set attack target"));
		SetAttackAction(false);
		Cast<AFantasyMissionGameState>(GetWorld()->GetGameState())->DoAttack(SelectedObjects[0], AttackTarget);
	}
	else
	{
		if (!Cast<AFantasyMissionCharacter>(Value->GetOwner()) || !bIsSelectingMultiple)
		{
			CloseSelection();
		}
		else if (SelectedObjects.Num() > 0)
		{
			// to avoid crash, make sure they're of same class.
			if (Value->GetOwner()->GetClass() != SelectedObjects[0]->GetOwner()->GetClass())
			{
				CloseSelection();
			}
			// IF FactionID is different when selecting multiple, remove all previous selection
			else if (Cast<AFantasyMissionCharacter>(Value->GetOwner())->FactionDataComponent->FactionData->FactionID != Cast<AFantasyMissionCharacter>(SelectedObjects[0]->GetOwner())->FactionDataComponent->FactionData->FactionID)
			{
				CloseSelection();
			}
		}
		SelectedObjects.Add(Value);
		Cast<AFantasyMissionHUD>(GetHUD())->SetActorsSelected(SelectedObjects);
	}
}

void AFantasyMissionPlayerController::RemoveSelectedObject(UActorSelectionComponent* Value)
{
	SelectedObjects.Remove((UActorSelectionComponent*)Value);
	if (!bIsSelectingMultiple)
	{
		CloseSelection();
		SelectedObjects.Add((UActorSelectionComponent*)Value);
	}
	else
	{
		Value->CheckIfSelected();
	}
	Cast<AFantasyMissionHUD>(GetHUD())->SetActorsSelected(SelectedObjects);
}

void AFantasyMissionPlayerController::CloseSelection()
{
	bMoveActionSelected = false;
	bAttackActionSelected = false;
	TArray<UActorSelectionComponent*> TempArray = SelectedObjects;
	SelectedObjects.Empty();
	for (UActorSelectionComponent* SelectedObject : TempArray)
	{
		SelectedObject->CheckIfSelected();
	}
	Cast<AFantasyMissionHUD>(GetHUD())->SetActorsSelected(SelectedObjects);
}

void AFantasyMissionPlayerController::OnSelectingMultipleTriggered()
{
	bIsSelectingMultiple = true;
}

void AFantasyMissionPlayerController::OnSelectingMultipleReleased()
{
	bIsSelectingMultiple = false;
}