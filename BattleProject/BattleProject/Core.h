#pragma once

/******************************************************************************
* ���������� ���������
*/
constexpr unsigned BattleMapWidth = 3;
constexpr unsigned BattleMapHeight = 4;
constexpr unsigned PartySize = 6;

/******************************************************************************
* ��������� ���
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