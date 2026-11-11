// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "FantasyMission/Components/ActorSelectionComponent.h"
#include "FantasyMission/Pawns/FantasyMissionCharacter.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/LocalPlayer.h"
#include "FantasyMissionGameState.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyMissionHUD.h"
#include "FantasyMissionPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AFantasyMissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFantasyMissionPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Touch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	//Movevement Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MovementInputAction;

	//Zoom Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomInputAction;

	//Camera movement action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraMovementAction;

	//Selecting multiple action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Selection, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectingMultipleAction;

	void AddSelectedObject(UActorSelectionComponent* Value);

	void RemoveSelectedObject(UActorSelectionComponent* Value);

	UFUNCTION(BlueprintCallable, category = Selection)
	void CloseSelection();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Selection, meta = (AllowPrivateAccess = "true"))
	UActorSelectionComponent* AttackTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Selection, meta = (AllowPrivateAccess = "true"))
	TArray<UActorSelectionComponent*> SelectedObjects;

	UFUNCTION(BlueprintCallable, category = Selection)
	FVector GetSelectedObjectLocation();

	UFUNCTION(BlueprintCallable, category = Movement)
	void SetMoveAction(bool NewValue);

	UFUNCTION(BlueprintCallable, category = Attack)
	void SetAttackAction(bool NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bMoveActionSelected;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackActionSelected;

	UFUNCTION(BlueprintCallable, category = Action)
	void SetUsedAction(UAction* NewAction);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
	UAction* UsedAction;

	UFUNCTION(BlueprintCallable, category = Action)
	bool CanSettle();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void OnInputMovement(const FInputActionValue& Value);
	void OnZoomTriggered(const FInputActionValue& Value);
	void OnCameraMovementTriggered();
	void OnCameraMovementReleased();
	void OnSelectingMultipleTriggered();
	void OnSelectingMultipleReleased();
	void UpdateUnitActionUI();

private:
	FVector CameraCachedDestination;
	FVector UnitCachedDestination;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	float MinZoomLength;
	float MaxZoomLength;
	float ZoomSpeed;
	bool bIsSelectingMultiple;
	bool bAllowMovement;
	AMapTile* DestinationMapTile;
};


