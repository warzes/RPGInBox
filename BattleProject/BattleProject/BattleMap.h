#pragma once

/******************************************************************************
* клетка поля битвы - 3x4 ячеек
*/
struct BattleCell
{
	union
	{
		Enemy* enemy;
		Hero* player;
	}; // указатель на участника боя
	EntityType type;

	enum class state
	{
		None,
		CurrentAction, // персонаж на этой клетке ожидает действия
		Target // персонаж этой клетки помечен как цель
	} currState = state::None;

	bool isAction = true; // может действовать

	void Draw()
	{
		if (type == EntityType::Free) // на клетке никого нет
		{
			printf("[ ]");
		}
		else
		{
			if (currState == state::None)
			{
				if (type == EntityType::Enemy && enemy)
					enemy->BattleDraw();
				else if (type == EntityType::Player && player)
					player->BattleDraw();
			}
			else if (currState == state::CurrentAction)
			{
				printf("[*]");
			}
			else if (currState == state::Target)
			{
				printf("[=]");
			}
		}
	}

	bool IsFree()
	{
		return  type == EntityType::Free || (enemy == nullptr && player == nullptr) || GetHP() <= 0;
	}

	int GetHP()
	{
		if (type == EntityType::Enemy && enemy != nullptr)
			return enemy->hp;
		else if (type == EntityType::Player && player != nullptr)
			return player->hp;
		return 0;
	}

	bool IsAction() // может ли этот персонаж действовать?
	{
		return isAction == true && !IsFree();
	}

	bool IsAlive() // персонаж жив?
	{
		return GetHP() > 0;
	}
};

/******************************************************************************
* поле битвы - 3x4 ячеек
*/
struct BattleMap
{
	void Init(HeroParty& playerParty, EnemyParty& enemyParty)
	{
		// растановка врагов
		for (unsigned i = 0; i < PartySize; i++)
		{
			unsigned x, y;
			Get2DIndex(i, x, y);
			cells[x][y].enemy = &enemyParty.enemies[i];
			cells[x][y].type = EntityType::Enemy;
		}
		// расстановка героев. Циклы отдельны так как кол-во врагов может быть не равно кол-ву героев
		for (unsigned i = 0; i < PartySize; i++)
		{
			unsigned x, y;
			Get2DIndex(i + PartySize, x, y);
			cells[x][y].player = &playerParty.heroes[i];
			cells[x][y].type = EntityType::Player;
		}
	}
	BattleCell cells[BattleMapWidth][BattleMapHeight];

	void ResetSelectState()
	{
		for (size_t x = 0; x < BattleMapWidth; x++)
		{
			for (size_t y = 0; y < BattleMapHeight; y++)
			{
				cells[x][y].currState = BattleCell::state::None;
			}
		}
	}

	void SetState(unsigned index, BattleCell::state newState)
	{
		unsigned x, y;
		Get2DIndex(index, x, y);
		SetState(x, y, newState);
	}
	void SetState(unsigned x, unsigned y, BattleCell::state newState)
	{
		cells[x][y].currState = newState;
	}

	void Draw()
	{
		// TODO: в проекте перевернуть массив
		for (size_t y = 0; y < BattleMapHeight; y++)
		{
			printf("\t");
			for (size_t x = 0; x < BattleMapWidth; x++)
			{
				cells[x][y].Draw();
			}
			printf("\n");
		}
	}

	bool IsAction(unsigned x, unsigned y) // может ли этот персонаж действовать?
	{
		return cells[x][y].IsAction();
	}

	bool IsAlive(unsigned x, unsigned y) // персонаж жив?
	{
		// TODO: проверка хп
		return cells[x][y].IsAlive();
	}

	void newRound()
	{
		// тут можно выкидывать мертвых, возвращать стейты и т.д.
	}

	bool IsPlayer(unsigned index)
	{
		unsigned x, y;
		Get2DIndex(index, x, y);
		return IsPlayer(x, y);
	}
	bool IsPlayer(unsigned x, unsigned y)
	{
		return cells[x][y].type == EntityType::Player && cells[x][y].player != nullptr;
	}
	bool IsEnemy(unsigned index)
	{
		unsigned x, y;
		Get2DIndex(index, x, y);
		return IsEnemy(x, y);
	}
	bool IsEnemy(unsigned x, unsigned y)
	{
		return cells[x][y].type == EntityType::Enemy && cells[x][y].enemy != nullptr;
	}
};