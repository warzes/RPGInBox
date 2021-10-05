#pragma once

class ResourceManager;

enum class PartyType
{
	Enemy,
	Hero
};

class ICreature
{
public:
	virtual ~ICreature() = default;

	virtual PartyType GetPartyType() const = 0;

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

enum class HeroClass
{
	Warrior,
	Cleric,
	Wizard
};

class Hero final : public ICreature
{
public:
	PartyType GetPartyType() const final { return PartyType::Hero; }

	HeroClass mainClass = HeroClass::Warrior;

	struct
	{
		int Strenght = 0;
		int Dexterity = 0;
		int Toughness = 0;
		int Knowledge = 0;
		int Willpower = 0;
	} attributes;	
};

class Enemy final : public ICreature
{
public:
	PartyType GetPartyType() const final { return PartyType::Enemy; }
};