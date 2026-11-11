// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitDataComponent.h"

// Sets default values for this component's properties
UUnitDataComponent::UUnitDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUnitDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUnitDataComponent::CreateUnitData(EUnitType UnitType)
{
	UUnit* NewUnit = NewObject<UUnit>();
	NewUnit->SetUnitType(UnitType);
	NewUnit->SetUnitValues();
	SetUnitData(NewUnit);
}


void UUnitDataComponent::SetUnitData(UUnit* NewUnitData)
{
	UnitData = NewUnitData;
}

void UUnitDataComponent::RemoveUnitActions(int Number)
{
	ActionsAvailable -= Number;
}

void UUnitDataComponent::SetUnitActions(int Number)
{
	ActionsAvailable = Number;
}

void UUnitDataComponent::SetElement(EActionElement NewElement)
{
	Element = NewElement;
}