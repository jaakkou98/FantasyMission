// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HeroData.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYMISSION_API UHeroData : public UObject
{
	GENERATED_BODY()

public:
	UHeroData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hero)
	FString Name;
	
};
