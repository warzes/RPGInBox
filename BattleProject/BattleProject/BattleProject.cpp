#include <iostream>

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
	char c = 'e';
};

struct EnemyParty
{
	Enemy enemys[PartySize];
};

struct Player
{
	char c = 'p';
};

struct PlayerParty
{
	Player player[PartySize];
};

/******************************************************************************
* поле битвы - 3x4 ячеек
*/
struct BattleMap
{
	void Init(PlayerParty& playerParty, EnemyParty& enemyParty)
	{
		cells[0][0].enemy = &enemyParty.enemys[0];
		cells[0][0].type = EntityType::Enemy;
		cells[1][0].enemy = &enemyParty.enemys[1];
		cells[1][0].type = EntityType::Enemy;
		cells[2][0].enemy = &enemyParty.enemys[2];
		cells[2][0].type = EntityType::Enemy;
		cells[0][1].enemy = &enemyParty.enemys[3];
		cells[0][1].type = EntityType::Enemy;
		cells[1][1].enemy = &enemyParty.enemys[4];
		cells[1][1].type = EntityType::Enemy;
		cells[2][1].enemy = &enemyParty.enemys[5];
		cells[2][1].type = EntityType::Enemy;

		cells[0][2].player = &playerParty.player[0];
		cells[0][2].type = EntityType::Player;
		cells[1][2].player = &playerParty.player[1];
		cells[1][2].type = EntityType::Player;
		cells[2][2].player = &playerParty.player[2];
		cells[2][2].type = EntityType::Player;
		cells[0][3].player = &playerParty.player[3];
		cells[0][3].type = EntityType::Player;
		cells[1][3].player = &playerParty.player[4];
		cells[1][3].type = EntityType::Player;
		cells[2][3].player = &playerParty.player[5];
		cells[2][3].type = EntityType::Player;	
	}
	struct cell
	{
		union
		{
			Enemy* enemy;
			Player* player;
		};
		EntityType type;
		bool isAction = true; // может действовать
	} cells[BattleMapWidth][BattleMapHeight];
};

/******************************************************************************
* обработчик действий игрока
*/
struct PlayerActionMachine
{
	void Run(BattleMap* map, unsigned curMemberX, unsigned curMemberY)
	{

	}
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

	void Frame()
	{
		// TODO: в проекте перевернуть массив
		for (size_t y = 0; y < BattleMapHeight; y++)
		{
			printf("\t");
			for (size_t x = 0; x < BattleMapWidth; x++)
			{
				char dc = ' ';

				unsigned curMemberX = 0;
				unsigned curMemberY = 0;
				battle.currentMemberInMap(curMemberX, curMemberY);
				if (curMemberX == x && curMemberY == y)
				{
					dc = '*';
				}
				else
				{
					if (map.cells[x][y].type == EntityType::Enemy)
					{
						dc = map.cells[x][y].enemy->c;
					}
					else if (map.cells[x][y].type == EntityType::Player)
					{
						dc = map.cells[x][y].player->c;
					}
				}
				
				printf("[%c]", dc);
			}
			printf("\n");
		}
	}

	BattleMap map;
	PlayerParty playerParty;
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
		battleMgr.battle.Run();
		system("PAUSE");
		break;
	}
	return 0;
}