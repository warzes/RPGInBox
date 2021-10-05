#pragma once

class ResourceManager;

enum class CreatureType
{
	Enemy,
	Hero
};

enum class HeroClass
{
	Warrior,
	Cleric,
	Wizard
};

class ICreature
{
public:
	virtual ~ICreature() = default;

	virtual CreatureType GetCreatureType() const = 0;

	// Get Stats
	virtual int GetHP() const noexcept { return stats.HP; }


	// TODO: сравнивать еще и со стейтом существа "мертв"
	bool IsAlive() const noexcept { return GetHP() > 0; }
	// TODO: проверять остальные стейты типа окаменения
	bool IsAction() const noexcept { return true; }

	std::shared_ptr<Texture2D> battleTexture = nullptr;

	struct Stats
	{
		int HP = 10;
		int MeleeAttack = 0;
		int PhysicsArmor = 0;
	} stats;
};

class Hero : public ICreature
{
public:
	CreatureType GetCreatureType() const final { return CreatureType::Hero; }

	struct
	{
		int Strenght = 0;
		int Dexterity = 0;
		int Toughness = 0;
		int Knowledge = 0;
		int Willpower = 0;
	} attributes;	
};

class Enemy : public ICreature
{
public:
	CreatureType GetCreatureType() const final { return CreatureType::Enemy; }
};