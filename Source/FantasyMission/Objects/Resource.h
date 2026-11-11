// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Resource.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Wood UMETA(DisplayName = "Wood"),
	Steel UMETA(DisplayName = "Steel"),
	Cattle UMETA(DisplayName = "Cattle"),
	Fish UMETA(DisplayName = "Fish"),
	Sheep UMETA(DisplayName = "Sheep"),
	Horses UMETA(DisplayName = "Horses"),
	Food UMETA(DisplayName = "Food"),
	Production UMETA(DisplayName = "Production"),
	Gold UMETA(DisplayName = "Gold")
};

UCLASS(BlueprintType)
class FANTASYMISSION_API UResource : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	int MoneyValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	EResourceType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	int Amount;

	UFUNCTION(BlueprintCallable, category = Resources)
	void SetResourceType(EResourceType NewType);

	UFUNCTION(BlueprintCallable, category = Resources)
	void SetResourceAmount(int NewAmount);

	UFUNCTION(BlueprintCallable, category = Resources)
	void SetResourceValues();
};
