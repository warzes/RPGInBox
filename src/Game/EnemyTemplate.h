#pragma once

#include "Creature.h"

class EnemyParty final
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr) noexcept;

	Enemy* grid[3][2] = { nullptr }; // расположение врагов. [1-3][0] - первый ряд; [1-3][1] - второй ряд
	std::vector<Enemy> enemys;
};