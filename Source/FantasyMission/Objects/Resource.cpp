// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

void UResource::SetResourceType(EResourceType NewType)
{
	Type = NewType;
}
void UResource::SetResourceAmount(int NewAmount)
{
	Amount = NewAmount;
}

void UResource::SetResourceValues()
{
	switch (Type)
	{
	case EResourceType::Wood:
		MoneyValue = 1;
		break;
	case EResourceType::Steel:
		MoneyValue = 3;
		break;
	case EResourceType::Cattle:
		MoneyValue = 2;
		break;
	case EResourceType::Fish:
		MoneyValue = 2;
		break;
	case EResourceType::Sheep:
		MoneyValue = 1;
		break;
	case EResourceType::Horses:
		MoneyValue = 3;
		break;
	case EResourceType::Food:
		MoneyValue = 0;
		break;
	case EResourceType::Production:
		MoneyValue = 0;
		break;
	case EResourceType::Gold:
		MoneyValue = 10;
		break;
	}
}

