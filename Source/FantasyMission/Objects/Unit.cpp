// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"


UUnit::UUnit()
{
	ActionList = TArray<EUnitAction>();
	ProductionCosts = TMap<EResourceType, int>();
	Level = 1;
	Experience = 0;
	LevelUpEXP = 100;
}

void UUnit::GainStatBoost(FString Stat)
{
	if (Stat.Equals("Actions"))
	{
		ActionAmount += 1;
	}
	else if (Stat.Equals("Attack"))
	{
		AttackModifier += 1;
	}
	else if (Stat.Equals("Defense"))
	{
		DefenseModifier += 1;
	}
	else if (Stat.Equals("Health"))
	{
		Health += 1;
		MaxHealth += 1;
	}
}

int UUnit::GetEXPYield()
{
	return EXPYield * Level;
}

void UUnit::GainExperience(int Yield, bool Boost)
{
	int Multiplier = 1;
	if (Boost)
	{
		Multiplier = 2;
	}
	Experience += Yield*Multiplier;
	while (Experience >= LevelUpEXP * Level)
	{
		GainLevel();
	}
}

void UUnit::GainLevel()
{
	Experience = Experience - LevelUpEXP * Level;
	Level += 1;
	AttackModifier = AttackModifier + Level;
	DefenseModifier = DefenseModifier + Level;
	Health = Health + Level;
	MaxHealth = MaxHealth + Level;

}

void UUnit::TakeDamage(int Damage)
{
	Health -= Damage;
	if (Health < 0)
	{
		Health = 0;
	}
}

void UUnit::Heal(int Amount)
{
	Health += Amount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void UUnit::SetUnitType(EUnitType NewType)
{
	Type = NewType;
}

void UUnit::SetUnitValues()
{
	switch (Type)
	{
	case EUnitType::Settler:
		Name = "Settler";
		ActionAmount = 4;
		AttackModifier = 1;
		DefenseModifier = 1;
		MaintenanceCost = 1;
		Health = 5;
		MaxHealth = 5;
		EXPYield = 10;
		ActionList.Add(EUnitAction::Move);
		ActionList.Add(EUnitAction::Settle);
		ProductionCosts.Add(EResourceType::Production, 5);
		break;
	case EUnitType::Scout:
		Name = "Scout";
		ActionAmount = 7;
		AttackModifier = 2;
		DefenseModifier = 2;
		MaintenanceCost = 1;
		Health = 7;
		MaxHealth = 7;
		EXPYield = 20;
		ActionList.Add(EUnitAction::Move);
		ActionList.Add(EUnitAction::Attack);
		ProductionCosts.Add(EResourceType::Production, 2);
		break;
	case EUnitType::Warrior:
		Name = "Warrior";
		ActionAmount = 5;
		AttackModifier = 4;
		DefenseModifier = 4;
		MaintenanceCost = 2;
		Health = 10;
		MaxHealth = 10;
		EXPYield = 40;
		ActionList.Add(EUnitAction::Move);
		ActionList.Add(EUnitAction::Attack);
		ProductionCosts.Add(EResourceType::Production, 3);
		break;
	case EUnitType::Archer:
		Name = "Archer";
		ActionAmount = 5;
		AttackModifier = 3;
		DefenseModifier = 3;
		MaintenanceCost = 2;
		Health = 8;
		MaxHealth = 8;
		EXPYield = 40;
		ActionList.Add(EUnitAction::Move);
		ActionList.Add(EUnitAction::Attack);
		ActionList.Add(EUnitAction::RangedAttack);
		ProductionCosts.Add(EResourceType::Production, 3);
		break;
	case EUnitType::Mage:
		Name = "Mage";
		ActionAmount = 4;
		AttackModifier = 6;
		DefenseModifier = 4;
		MaintenanceCost = 3;
		Health = 6;
		MaxHealth = 6;
		EXPYield = 50;
		ActionList.Add(EUnitAction::Move);
		ActionList.Add(EUnitAction::Attack);
		ActionList.Add(EUnitAction::MagicAttack);
		ProductionCosts.Add(EResourceType::Production, 4);
		break;
	}
}