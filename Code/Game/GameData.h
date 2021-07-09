#pragma once

#include "PlayerUnit.h"
#include "WorldMap.h"

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
	std::unique_ptr<WorldMap> m_world;

	PlayerState playerState;
	Directions::Value playerFacing;
	Vector2DI playerPosition;
	Directions directions;
	// used to keep track of player action progress
	float radiansRotated;
	float moveProgress;
	bool wallBumped; // used to keep track of where we are in the wallBump
};