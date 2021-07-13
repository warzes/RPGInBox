#include "stdafx.h"
#include "Player.h"
//-----------------------------------------------------------------------------
bool Party::Move(const Vector3& pos) noexcept
{
	m_position.x = (float)pos.x;
	m_position.y = (float)pos.z;
	printf("PP = %d %d\n", m_position.x, m_position.y);

	if (m_oldPosition.x != m_position.x || m_oldPosition.y != m_position.y)
	{
		m_oldPosition = m_position;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------