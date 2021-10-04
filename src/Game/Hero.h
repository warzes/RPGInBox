#pragma once

#include "Stats.h"
#include "oCreature.h"

enum class HeroClass
{
	Warrior,
	Cleric,
	Wizard
};

class oHero final : public oICreature
{
public:
	CreatureType GetType() const noexcept final { return CreatureType::Hero; }

	// Base
	HeroClass characterClass = HeroClass::Warrior;
	std::string name;

	// Attributes
	struct
	{
		int strength = 0;
		int toughness = 0;
		int dexterity = 0;
		int knowledge = 0; // TODO: интеллект?
		int willpower = 0;
	} attributes;

	// Statistics
	struct
	{
		Stats hp;
		Stats mp;
		int meleeAttack = 0;
		int shootAttack = 0;
		int magicAttack = 0;
		int armor = 0;
		int magicDefense = 0;
	} statistics;

	// Resistances
	struct
	{
		int fire = 0;
		int cold = 0;
		int lightning = 0;
		int acid = 0;
	} resistances;

	// Immunities
	struct 
	{
		bool poison = false;
		bool confusion = false;
		bool paralysis = false;
		bool fear = false;
		bool mesmerize = false;
		bool petrify = false;
	} immunities;


	// Condition
	struct
	{
		bool dead = false;
		bool asleep = false;
	} condition;

	// Talents
	// TODO:

	// Equipment
	// TODO:
};