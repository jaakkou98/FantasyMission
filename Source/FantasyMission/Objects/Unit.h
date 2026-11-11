// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.h"
#include "Resource.h"
#include "Unit.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Settler UMETA(DisplayName = "Settler"),
	Scout UMETA(DisplayName = "Scout"),
	Warrior UMETA(DisplayName = "Warrior"),
	Archer UMETA(DisplayName = "Archer"),
	Mage UMETA(DisplayName = "Mage"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EUnitType, EUnitType::Count);

UCLASS(BlueprintType)
class FANTASYMISSION_API UUnit : public UObject
{
	GENERATED_BODY()

public:
	UUnit(); 

	UFUNCTION(BlueprintCallable, category = Units)
	void Heal(int Amount);

	UFUNCTION(BlueprintCallable, category = Units)
	void TakeDamage(int Damage);

	UFUNCTION(BlueprintCallable, category = Units)
	void GainExperience(int Yield, bool Boost);

	UFUNCTION(BlueprintCallable, category = Units)
	int GetEXPYield();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int ActionAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int EXPYield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int Experience;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int LevelUpEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	EUnitType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int AttackModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int DefenseModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int MaintenanceCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	TMap<EResourceType, int> ProductionCosts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	TArray<EUnitAction> ActionList;

	UFUNCTION(BlueprintCallable, category = Units)
	void SetUnitValues();

	UFUNCTION(BlueprintCallable, category = Units)
	void SetUnitType(EUnitType NewType);

	UFUNCTION(BlueprintCallable, category = Units)
	void GainStatBoost(FString Stat);
private:
	void GainLevel();
};
