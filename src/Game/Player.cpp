#include "stdafx.h"
#include "Player.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void Player::StartDefaultParty() noexcept
{
	m_party.heroes.resize(MaxNumCharacterInPlayerParty);
}
//-----------------------------------------------------------------------------