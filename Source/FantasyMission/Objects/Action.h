// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Element.h"
#include "Action.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EUnitAction : uint8
{
	Move UMETA(DisplayName = "Move"),
	Settle UMETA(DisplayName = "Settle"),
	Cancel UMETA(DisplayName = "Cancel"),
	Produce UMETA(DisplayName = "Produce"),
	Attack UMETA(DisplayName = "Attack"),
	RangedAttack UMETA(DisplayName = "Ranged Attack"),
	MagicAttack UMETA(DisplayName = "Magic Attack"),
	HeroAttack UMETA(DisplayName = "Hero Attack"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EUnitAction, EUnitAction::Count);

UCLASS(BlueprintType)
class FANTASYMISSION_API UAction : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, category = Actions)
	void SetActionElement(EActionElement NewElement);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	EActionElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	EUnitAction Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	int ActionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	int ActionCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	bool ConsumeUnit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actions)
	bool MultiUse;

	UFUNCTION(BlueprintCallable, category = Actions)
	void SetActionType(EUnitAction NewType);

	UFUNCTION(BlueprintCallable, category = Actions)
	void SetActionValues();
};
