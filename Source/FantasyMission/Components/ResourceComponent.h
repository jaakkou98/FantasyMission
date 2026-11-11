// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FantasyMission/Objects/Resource.h"
#include "ResourceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYMISSION_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resources)
	TArray<UResource*> Resources;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, category = Resources)
	void AddResourceOfType(EResourceType ResourceType);

	UFUNCTION(BlueprintCallable, category = Resources)
	void RemoveResource(UResource* NewResource);
		
};
