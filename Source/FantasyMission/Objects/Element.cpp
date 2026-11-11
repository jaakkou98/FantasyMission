// Fill out your copyright notice in the Description page of Project Settings.


#include "Element.h"

void UElement::SetElementType(EActionElement NewType)
{
	Type = NewType;
}

void UElement::SetElementValues()
{
	switch (Type)
	{
	case EActionElement::Fire:
		Strength = EActionElement::Wind;
		Weakness = EActionElement::Water;
		break;
	case EActionElement::Water:
		Strength = EActionElement::Fire;
		Weakness = EActionElement::Earth;
		break;
	case EActionElement::Earth:
		Strength = EActionElement::Water;
		Weakness = EActionElement::Wind;
		break;
	case EActionElement::Wind:
		Strength = EActionElement::Earth;
		Weakness = EActionElement::Fire;
		break;
	case EActionElement::Neutral:
		Strength = EActionElement::None;
		Weakness = EActionElement::None;
		break;
	}
}
