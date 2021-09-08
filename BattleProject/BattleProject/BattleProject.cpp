#include <iostream>
#include "Core.h"
#include "BattleMap.h"
#include "BattleMachine.h"

/******************************************************************************
* менеджер битвы
*/
struct BattleManager
{
	BattleManager()
	{
		// заполнение боевой карты персонажами
		map.Init(playerParty, enemyPlayer);
		// запуск механики битвы
		battle.Start(&map);
	}

	void Update()
	{
		battle.Update();
	}

	void Frame()
	{
		map.Draw();
	}

	BattleMap map;
	BattleMachine battle;
	HeroParty playerParty;
	EnemyParty enemyPlayer;
};

int main()
{
	setlocale(LC_ALL, "");
	
	BattleManager battleMgr;

	unsigned id = 11;
	unsigned x, y;
	Get2DIndex(id, x, y);
	std::cout << x << " " << y << std::endl;
	y = id / BattleMapWidth;
	x = id % BattleMapWidth;
	std::cout << x << " " << y << std::endl;

	std::cout << 10%3 << std::endl;

	while (1)
	{
		system("CLS");		
		battleMgr.Frame();
		battleMgr.Update();
		system("PAUSE");
		//break;
	}
	return 0;
}