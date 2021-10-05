#include "stdafx.h"
#include "EnemyTemplate.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void EnemyParty::CreateDefaultParty(ResourceManager* resourceMgr) noexcept
{
	enemys.clear();
	enemys.resize(MaxNumCharacterInEnemyParty);

	enemys[0].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[1].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[2].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[3].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[4].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");
	enemys[5].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-goblin.png");

	// ������ ���
	grid[0][0] = &enemys[0];
	grid[1][0] = &enemys[1];
	grid[2][0] = &enemys[2];

	// ������ ���
	grid[0][1] = &enemys[3];
	grid[1][1] = &enemys[4];
	grid[2][1] = &enemys[5];
}
//-----------------------------------------------------------------------------