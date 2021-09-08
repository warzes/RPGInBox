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
		cells[0][0].enemy = &enemyParty.enemies[0];
		cells[0][0].type = EntityType::Enemy;
		cells[1][0].enemy = &enemyParty.enemies[1];
		cells[1][0].type = EntityType::Enemy;
		cells[2][0].enemy = &enemyParty.enemies[2];
		cells[2][0].type = EntityType::Enemy;
		cells[0][1].enemy = &enemyParty.enemies[3];
		cells[0][1].type = EntityType::Enemy;
		cells[1][1].enemy = &enemyParty.enemies[4];
		cells[1][1].type = EntityType::Enemy;
		cells[2][1].enemy = &enemyParty.enemies[5];
		cells[2][1].type = EntityType::Enemy;

		cells[0][2].player = &playerParty.heroes[0];
		cells[0][2].type = EntityType::Player;
		cells[1][2].player = &playerParty.heroes[1];
		cells[1][2].type = EntityType::Player;
		cells[2][2].player = &playerParty.heroes[2];
		cells[2][2].type = EntityType::Player;
		cells[0][3].player = &playerParty.heroes[3];
		cells[0][3].type = EntityType::Player;
		cells[1][3].player = &playerParty.heroes[4];
		cells[1][3].type = EntityType::Player;
		cells[2][3].player = &playerParty.heroes[5];
		cells[2][3].type = EntityType::Player;
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
};