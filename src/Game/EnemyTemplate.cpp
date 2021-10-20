#include "stdafx.h"
#include "EnemyTemplate.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void EnemyParty::CreateDefaultParty(ResourceManager* resourceMgr, size_t number) noexcept
{
	enemys.clear();
	enemys.resize(number);

	enemys[0].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[1].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[2].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[3].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[4].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[5].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");

	// второй ряд // TODO: поменять местами
	grid[0][0] = &enemys[0];
	grid[1][0] = &enemys[1];
	grid[2][0] = &enemys[2];

	// первый ряд // TODO: поменять местами
	//grid[0][1] = &enemys[3];
	//grid[1][1] = &enemys[4];
	//grid[2][1] = &enemys[5];
}
//-----------------------------------------------------------------------------