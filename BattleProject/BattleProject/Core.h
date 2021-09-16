#pragma once

/******************************************************************************
* Глобальные константы
*/
constexpr unsigned BattleMapWidth = 3;
constexpr unsigned BattleMapHeight = 4;
constexpr unsigned PartySize = 6;


struct Point
{
	int x = 0;
	int y = 0;
};

/******************************************************************************
* Участники боя
*/
enum class EntityType
{
	Enemy,
	Player,
	Free
};

struct ICreature
{
	virtual ~ICreature() = default;

	virtual int GetHP() const noexcept = 0;

	int hp = 10;
};

struct Enemy final : public ICreature
{
	int att = 1;
	
	int GetHP() const noexcept final
	{
		return hp;
	}

	void BattleDraw()
	{
		printf("[e]");
	}
};

struct Hero final : public ICreature
{
	int att = 1;

	int GetHP() const noexcept final
	{
		return hp;
	}

	void BattleDraw()
	{
		printf("[p]");
	}
};

struct EnemyParty
{
	Enemy enemies[PartySize];
};

struct HeroParty
{
	Hero heroes[PartySize];
};

inline void Get2DIndex(unsigned id, unsigned& x, unsigned& y)
{
	x = id % BattleMapWidth;
	y = id / BattleMapWidth;
}