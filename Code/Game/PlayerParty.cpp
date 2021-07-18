#include "stdafx.h"
#include "PlayerParty.h"
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