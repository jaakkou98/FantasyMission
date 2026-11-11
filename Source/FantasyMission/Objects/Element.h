// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Element.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EActionElement : uint8
{
	Fire UMETA(DisplayName = "Fire"),
	Water UMETA(DisplayName = "Water"),
	Earth UMETA(DisplayName = "Earth"),
	Wind UMETA(DisplayName = "Wind"),
	Neutral UMETA(DisplayName = "Neutral"),
	None UMETA(DisplayName = "None"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EActionElement, EActionElement::Count)

UCLASS()
class FANTASYMISSION_API UElement : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Elements)
	EActionElement Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Elements)
	EActionElement Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Elements)
	EActionElement Weakness;

	void SetElementType(EActionElement NewType);

	void SetElementValues();
};
