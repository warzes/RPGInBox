#pragma once

#include "Creature.h"

class EnemyParty final
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr, size_t number) noexcept;

	Enemy* grid[3][2] = { nullptr }; // расположение врагов. [0-2][0] - первый ряд; [0-2][1] - второй ряд
	std::vector<Enemy> enemys;
};