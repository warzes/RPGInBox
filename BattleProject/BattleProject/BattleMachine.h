#pragma once

/******************************************************************************
* обработчик действий игрока
*/
struct PlayerActionMachine
{
	void Run(BattleMap* map, unsigned index)
	{
		unsigned curMemberX, curMemberY;
		Get2DIndex(index, curMemberX, curMemberY);
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
	void Start(BattleMap* nmap)
	{
		map = nmap;
		newRound(); // в начале боя сразу же выполняется начало нового раунда
		SetCurrentMember(7); // индекс текущего персонажа. временно
	}

	void newRound()
	{
		map->newRound();
	}

	void SetCurrentMember(unsigned id)
	{
		currentMember = id; // индекс текущего персонажа. временно
		map->SetState(currentMember - 1, BattleCell::state::CurrentAction);
	}

	void Update()
	{
		if (map->IsPlayer(currentMember - 1))
		{
			playerAction.Run(map, currentMember - 1);
		}
		else if (map->IsEnemy(currentMember - 1))
		{
			nextMember();
		}
	}

	BattleMap* map;
	unsigned currentMember = 0; // от 1 до 12. 0 - нет персонажа или ошибка, не забывать отнимать единицу


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
			//unsigned curMemberX = 0;
			//unsigned curMemberY = 0;
			//currentMemberInMap(curMemberX, curMemberY);
			//if (map->cells[curMemberX][curMemberY].isAction == false || map->cells[curMemberX][curMemberY].type == EntityType::Free)
			//	nextMember();
		}
	}

	PlayerActionMachine playerAction;
};