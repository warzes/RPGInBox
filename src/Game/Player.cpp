#include "stdafx.h"
#include "Player.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void Player::CreateDefaultParty(ResourceManager* resourceMgr) noexcept
{
	heroes.resize(MaxNumCharacterInPlayerParty);

	// первый герой
	heroes[0].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");

	// второй герой
	heroes[1].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");

	// третий герой
	heroes[2].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");

	// четвертый герой
	heroes[3].battleTexture = resourceMgr->GetTexture("../data/temp/textures/character/mon-rogue3.png");
	heroes[3].mainClass == HeroClass::Wizard;

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
bool Player::Move(const Point2& pos) noexcept
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	//printf("PP = %d %d\n", m_position.x, m_position.y);

	if (m_oldPosition.x != m_position.x || m_oldPosition.y != m_position.y)
	{
		m_oldPosition = m_position;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------