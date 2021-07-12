#pragma once

#include "PlayerUnit.h"

enum class PlayerState
{
	movingForward, movingBackward,
	wallBumpForward, wallBumpBackward,
	turningLeft, turningRight,
	standing
};

class GameData
{
public:
	void Init();

	// player data
	std::vector<PlayerUnit> party;
};