#include "stdafx.h"
#include "GameData.h"

void GameData::Init()
{
	// TODO load this from a save file or something
	// TEMP for testing we'll hardcode the party here for now
	party.push_back(PlayerUnit(L"John", Stats(10, 3, 3, 3, 4, 74)));
	party.push_back(PlayerUnit(L"Clarence", Stats(8, 3, 3, 3, 4, 55)));
	party.push_back(PlayerUnit(L"Samson", Stats(5, 12, 3, 3, 5, 40)));
	party.push_back(PlayerUnit(L"Darian", Stats(7, 7, 3, 3, 7, 52)));
	party.push_back(PlayerUnit(L"Oak", Stats(4, 15, 3, 3, 6, 35)));

	party[0].addSkill(Skills::cleave);
	party[1].addSkill(Skills::cleave);
	party[2].addSkill(Skills::cleave);
	party[2].addSkill(Skills::fireball);
	party[3].addSkill(Skills::fireball);
	party[3].addSkill(Skills::cleave);
	party[4].addSkill(Skills::fireball);

	m_world.reset(new WorldMap("dungeon.json"));

	// initial player state
	playerState = PlayerState::standing;
	playerFacing = m_world->getStartFacing();
	radiansRotated = 0;
	moveProgress = 0;
	wallBumped = false;

	playerPosition = { 0, 0 };

	m_world->initializeScene();

	Vector3 camPosition;
	Vector3 initialTarget;
	Directions::Value initialDirection;

	bool totalReset = true;
	if (totalReset)
	{
		camPosition = m_world->getCamStartPosition();
		playerPosition = m_world->getStartPosition();

		initialDirection = m_world->getStartFacing();
	}
	else
	{
		camPosition = m_world->getPosition(playerPosition.x, playerPosition.y);

		initialDirection = playerFacing;
	}

	//camera->setPosition(camPosition);
	//camera->setTarget(camera->getPosition() + directions.getVectorFloat(initialDirection));


}
