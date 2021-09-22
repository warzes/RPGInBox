#pragma once

#include "Stats.h"
#include "Creature.h"

class oEnemy final : public ICreature
{
public:
	std::string name;
	CreatureType GetType() const noexcept final { return CreatureType::Enemy; }

	// Statistics
	struct
	{
		Stats hp = 0;
		Stats mp = 0;
	} statistics;
};