// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.h"
#include "PowerUp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	StatBoost UMETA(DisplayName = "Attribute Boost"),
	Action UMETA(DisplayName = "Action"),
	LevelUp UMETA(DisplayName = "Level Up"),
	Hero UMETA(DisplayName = "Become Hero")
};

UCLASS(BlueprintType)
class FANTASYMISSION_API UPowerUp : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, category = PowerUps)
	void SetPowerUpType(EPowerUpType NewType);

	UFUNCTION(BlueprintCallable, category = PowerUps)
	void SetPowerUpValues();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPowerUpType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUnitAction Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StatBoost;

private:
	void SetAction();
	void SetStatBoost();
};
