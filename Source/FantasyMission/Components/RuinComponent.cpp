// Fill out your copyright notice in the Description page of Project Settings.


#include "RuinComponent.h"

// Sets default values for this component's properties
URuinComponent::URuinComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetPowerUp();
}


// Called when the game starts
void URuinComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URuinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URuinComponent::SetPowerUp()
{
	PowerUp = NewObject<UPowerUp>();
}