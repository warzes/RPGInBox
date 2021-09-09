#pragma once

/******************************************************************************
* обработчик действий игрока
*/
struct PlayerActionMachine
{
	void Run(BattleRules& rules, BattleMap* map, unsigned index)
	{
		unsigned curMemberX, curMemberY;
		Get2DIndex(index, curMemberX, curMemberY);

		if (state == State::selectAction)
		{
			printf("\nвыберите действие: 1) ближняя атака; 2) дальняя атака; 3) пропуск хода\n");
			
			std::cin >> inputValue;
			if (inputValue == 1) // игрок выбрал ближнюю атаку
			{
				inputValue = 0;
				if (curMemberY == 2) // бить ближней атакой может только ближний ряд игрока
				{
					int numSel = 0;
					if (map->SetState(0, 1, BattleCell::state::Target))
						numSel++;
					if (map->SetState(1, 1, BattleCell::state::Target))
						numSel++;
					if (map->SetState(2, 1, BattleCell::state::Target))
						numSel++;
					if (numSel == 0) // а в первом ряду нет dhfujd, смотрим второй ряд
					{
						if (map->SetState(0, 0, BattleCell::state::Target))
							numSel++;
						if (map->SetState(1, 0, BattleCell::state::Target))
							numSel++;
						if (map->SetState(2, 0, BattleCell::state::Target))
							numSel++;
					}
					if (numSel == 0) // и во втором ряду никого нет
					{
					}
					else
					{
						// все ок, цели помечены, теперь переходим в режим ожидания выбора одной цели
						state = State::selectMeleeTarget;
					}
				}
			}
		}
		else if (state == State::selectMeleeTarget)
		{
			int num = 0;
			std::vector<Point> target;
			if (map->IsAlive(0, 1))
			{
				num++;
				target.push_back({ 0, 1 });
			}				
			if (map->IsAlive(1, 1))
			{
				num++;
				target.push_back({ 1, 1 });
			}
			if (map->IsAlive(2, 1))
			{
				num++;
				target.push_back({ 2, 1 });
			}

			if (num == 0) // в первом ряду врагов нет, смотрим второй ряд
			{
				if (map->IsAlive(0, 0))
				{
					num++;
					target.push_back({ 0, 0 });
				}
				if (map->IsAlive(1, 0))
				{
					num++;
					target.push_back({ 1, 0 });
				}
				if (map->IsAlive(2, 0))
				{
					num++;
					target.push_back({ 2, 0 });
				}
			}

			printf("\nвыберите цель: 1-%d; или 0 для отмены\n", num);
			std::cin >> inputValue;
			if (inputValue == 0) // отмена
			{
				state = State::selectAction;
				map->ResetSelectState();
			}
			else 
			{

			}
		}
	}

	int inputValue = 0;

	enum class State
	{
		selectAction,     // ожидание выбора действия

		selectMeleeTarget // ожидание выбора цели ближнего удара
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
		SetCurrentMember(8); // индекс текущего персонажа. временно
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
			playerAction.Run(rules, map, currentMember - 1);
		}
		else if (map->IsEnemy(currentMember - 1))
		{
			//nextMember();
		}
	}

	BattleRules rules;
	BattleMap* map = nullptr;
	unsigned currentMember = 0; // от 1 до 12. 0 - нет персонажа или ошибка, не забывать отнимать единицу

	//void nextMember()
	//{
	//	// тут какие-то условия по которым выбирать следующего дейстующего участника боя
	//	currentMember++;
	//	if (currentMember > PartySize * 2) // закончились участники боя, начинаем новый раунд
	//	{
	//		newRound();
	//		currentMember = 0;
	//	}
	//	else // а этот участник боя вообще может действовать? Если нет, снова запускаем поиск
	//	{
	//		//unsigned curMemberX = 0;
	//		//unsigned curMemberY = 0;
	//		//currentMemberInMap(curMemberX, curMemberY);
	//		//if (map->cells[curMemberX][curMemberY].isAction == false || map->cells[curMemberX][curMemberY].type == EntityType::Free)
	//		//	nextMember();
	//	}
	//}

	PlayerActionMachine playerAction;
};