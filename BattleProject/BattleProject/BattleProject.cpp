#include <iostream>
#include "Core.h"
#include "BattleMap.h"

/******************************************************************************
* обработчик действий игрока
*/
struct PlayerActionMachine
{
	void Run(BattleMap* map, unsigned curMemberX, unsigned curMemberY)
	{
		if (state == State::selectAction)
		{
			printf("выберите действие: 1) ближняя атака; 2) дальняя атака; 3) пропуск хода");
			int in = 0;
			std::cin >> in;
			if (in == 1) // игрок выбрал ближнюю атаку
			{
				if (curMemberY == 2) // бить ближней атакой может только ближний ряд игрока
				{
					int numSel = 0;
					if (map->IsAlive(0, 1))
					{
						map->cells[0][1].currState = BattleCell::state::Target;
						numSel++;
					}
					if (map->IsAlive(1, 1))
					{
						map->cells[1][1].currState = BattleCell::state::Target;
						numSel++;
					}
					if (map->IsAlive(2, 1))
					{
						map->cells[2][1].currState = BattleCell::state::Target;
						numSel++;
					}

					if (numSel == 0) // а в первом ряду врага никого нет, смотрим второй ряд
					{
						if (map->IsAlive(0, 0))
						{
							map->cells[0][0].currState = BattleCell::state::Target;
							numSel++;
						}
						if (map->IsAlive(1, 0))
						{
							map->cells[1][0].currState = BattleCell::state::Target;
							numSel++;
						}
						if (map->IsAlive(2, 0))
						{
							map->cells[2][0].currState = BattleCell::state::Target;
							numSel++;
						}
					}

					if (numSel == 0) // и во втором ряду никого нет
					{
					}
					else
					{
						state = State::selectMeleeTarget;
					}
				}
			}
			else if (in == 2) // игрок выбрал пропуск хода
			{

			}
		}
	}

	enum class State
	{
		selectAction,
		selectMeleeTarget
	} state = State::selectAction;
};

/******************************************************************************
* обработчик битвы
*/
struct BattleMachine
{
	void Start(BattleMap *nmap)
	{	
		map = nmap;
		newRound();
		currentMember = 7; // бой пока начинать с игрока
	}

	void Run()
	{
		unsigned curMemberX = 0;
		unsigned curMemberY = 0;
		currentMemberInMap(curMemberX, curMemberY);

		if (map->cells[curMemberX][curMemberY].type == EntityType::Player)
		{
			playerAction.Run(map, curMemberX, curMemberY);
		}
		else if (map->cells[curMemberX][curMemberY].type == EntityType::Enemy)
		{
			nextMember();
		}
	}

	BattleMap* map;
	unsigned currentMember = 0;

	void newRound()
	{
		for (size_t x = 0; x < BattleMapWidth; x++)
		{
			for (size_t y = 0; y < BattleMapHeight; y++)
			{
				map->cells[x][y].isAction = true;
			}
		}
	}
	void currentMemberInMap(unsigned& x, unsigned& y)
	{
		switch (currentMember)
		{
		case 1:  x = 0; y = 0; break;
		case 2:  x = 1; y = 0; break;
		case 3:  x = 2; y = 0; break;
		case 4:  x = 0; y = 1; break;
		case 5:  x = 1; y = 1; break;
		case 6:  x = 2; y = 1; break;
		case 7:  x = 0; y = 2; break;
		case 8:  x = 1; y = 2; break;
		case 9:  x = 2; y = 2; break;
		case 10: x = 0; y = 3; break;
		case 11: x = 1; y = 3; break;
		case 12: x = 2; y = 3; break;
		default: break;
		}
	}
	void nextMember()
	{
		// тут какие-то условия по которым выбирать следующего дейстующего участника боя
		currentMember++;
		if (currentMember > PartySize * 2) // закончились участники боя, начинаем новый раунд
		{
			newRound();
			currentMember = 0;
		}
		else // а этот участник боя вообще может действовать? Если нет, снова запускаем поиск
		{
			unsigned curMemberX = 0;
			unsigned curMemberY = 0;
			currentMemberInMap(curMemberX, curMemberY);
			if (map->cells[curMemberX][curMemberY].isAction == false || map->cells[curMemberX][curMemberY].type == EntityType::Free)
				nextMember();
		}
	}

	PlayerActionMachine playerAction;
};

/******************************************************************************
* менеджер битвы
*/
struct BattleManager
{
	BattleManager()
	{
		map.Init(playerParty, enemyPlayer);
		battle.Start(&map);
	}

	void Update()
	{
		battle.Run();
	}

	void Frame()
	{
		map.Draw();
	}

	BattleMap map;
	HeroParty playerParty;
	EnemyParty enemyPlayer;

	BattleMachine battle;
};

int main()
{
	setlocale(LC_ALL, "");
	
	BattleManager battleMgr;

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