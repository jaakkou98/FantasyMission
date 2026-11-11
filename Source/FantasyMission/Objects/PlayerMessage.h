// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerMessage.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerMessageType : uint8
{
	WarStart UMETA(DisplayName = "War Starts"),
	WarEnd UMETA(DisplayName = "Suggest War End"),
	AllianceStart UMETA(DisplayName = "Suggest New Alliance"),
	AllianceEnd UMETA(DisplayName = "Alliance Ends"),
	PeaceAccepted UMETA(DisplayName = "Peace Accepted"),
	AllianceAccepted UMETA(DisplayName = "Alliance Accepted"),
	PeaceRejected UMETA(DisplayName = "Peace Rejected"),
	AllianceRejected UMETA(DisplayName = "Alliance Rejected"),
	Eliminated UMETA(DisplayName = "Eliminated"),
	Victory UMETA(DisplayName = "Victory")
};

UCLASS(BlueprintType)
class FANTASYMISSION_API UPlayerMessage : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Sender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Receiver;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerMessageType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bChoice;

	UFUNCTION(BlueprintCallable, category = PlayerMessage)
	void SetMessageType(EPlayerMessageType NewType);

	UFUNCTION(BlueprintCallable, category = PlayerMessage)
	void SetMessageValues(FString NewSender, FString NewReceiver);
};
