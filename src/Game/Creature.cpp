#include "stdafx.h"
#include "Creature.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void Player::CreateDefaultParty() noexcept
{
	heroes.resize(MaxNumCharacterInPlayerParty);

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