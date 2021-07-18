#pragma once

enum class CharacterClass
{
	Warrior,
	Cleric,
	Wizard
};

class Character final
{
public:
	// base
	CharacterClass characterClass = CharacterClass::Warrior;
	char name[MaxGameNameTextSize];

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
		int hp = 0;
		int mp = 0;
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