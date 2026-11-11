// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"


void UAction::SetActionType(EUnitAction NewType)
{
	Type = NewType;
}

void UAction::SetActionElement(EActionElement NewElement)
{
	Element = NewElement;
}

void UAction::SetActionValues()
{
	switch (Type)
	{
	case EUnitAction::Move:
		ActionCost = 0;
		ActionRange = 0;
		ConsumeUnit = false;
		MultiUse = true;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::Settle:
		ActionCost = 0;
		ActionRange = 0;
		ConsumeUnit = true;
		MultiUse = false;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::Cancel:
		ActionCost = 0;
		ActionRange = 0;
		ConsumeUnit = false;
		MultiUse = true;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::Produce:
		ActionCost = 0;
		ActionRange = 0;
		ConsumeUnit = false;
		MultiUse = false;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::Attack:
		ActionCost = 1;
		ActionRange = 1;
		ConsumeUnit = false;
		MultiUse = false;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::RangedAttack:
		ActionCost = 1;
		ActionRange = 2;
		ConsumeUnit = false;
		MultiUse = false;
		Element = EActionElement::Neutral;
		break;
	case EUnitAction::MagicAttack:
		ActionCost = 2;
		ActionRange = 3;
		ConsumeUnit = false;
		MultiUse = false;
		Element = EActionElement::None;
		break;
	case EUnitAction::HeroAttack:
		ActionCost = 3;
		ActionRange = 4;
		ConsumeUnit = false;
		MultiUse = false;
		Element = EActionElement::None;
		break;
	}
}
