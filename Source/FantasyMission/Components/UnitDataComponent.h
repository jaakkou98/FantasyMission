// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FantasyMission/Objects/Unit.h"
#include "UnitDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYMISSION_API UUnitDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitDataComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	UUnit* UnitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	int ActionsAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	EActionElement Element;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, category = Unit)
	void SetElement(EActionElement NewElement);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, category = Unit)
	void CreateUnitData(EUnitType UnitType);

	UFUNCTION(BlueprintCallable, category = Unit)
	void SetUnitData(UUnit* NewUnitData);

	UFUNCTION(BlueprintCallable, category = Actions)
	void RemoveUnitActions(int Number);

	UFUNCTION(BlueprintCallable, category = Actions)
	void SetUnitActions(int Number);
};
