#include "stdafx.h"
#include "Player.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void Player::CreateDefaultParty(ResourceManager* resourceMgr) noexcept
{
	heroes.resize(MaxNumCharacterInPlayerParty);

	heroes[0].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");
	heroes[1].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");
	heroes[2].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");
	heroes[3].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");

	// первый ряд
	grid[0][0] = &heroes[0];
	grid[1][0] = &heroes[1];
	grid[2][0] = &heroes[2];

	// второй ряд
	grid[0][1] = nullptr;
	grid[1][1] = &heroes[3];
	grid[2][1] = nullptr;
}
//-----------------------------------------------------------------------------