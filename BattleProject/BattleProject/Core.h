﻿#pragma once

/******************************************************************************
* Глобальные константы
*/
constexpr unsigned BattleMapWidth = 3;
constexpr unsigned BattleMapHeight = 4;
constexpr unsigned PartySize = 6;

/******************************************************************************
* Участники боя
*/
enum class EntityType
{
	Enemy,
	Player,
	Free
};

struct Enemy
{
	int hp = 10;

	void BattleDraw()
	{
		printf("[e]");
	}
};

struct Hero
{
	int hp = 10;

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
	x = id;
	y = 0;
	while (x >= BattleMapWidth)
	{
		y++;
		x = x - BattleMapWidth;
	}
}