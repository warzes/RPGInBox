#pragma once

#include "Creature.h"

// нет отдельного класса под партию - не нужен, передавать сразу игрока
class Player final
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr) noexcept;


	Hero* grid[3][2] = { nullptr }; // расположение героев. [1-3][0] - первый ряд; [1-3][1] - второй ряд
	std::vector<Hero> heroes;
};