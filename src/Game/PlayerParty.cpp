#include "stdafx.h"
#include "PlayerParty.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
PlayerParty::PlayerParty()
{
	positionCharactersInParty[0] = { 0, 0 };
	positionCharactersInParty[1] = { 1, 0 };
	positionCharactersInParty[2] = { 2, 0 };
	positionCharactersInParty[3] = { 1, 1 };

	for (int i = 0; i < MaxNumCharacterInPlayerParty; i++)
		characters[i].stats.HP = 100;
}
//-----------------------------------------------------------------------------
bool PlayerParty::Move(const Point2& pos) noexcept
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