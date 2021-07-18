#include "stdafx.h"
#include "GameEnvironment.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void GameEnvironment::NextStep(const World& world)
{
	m_dangerLevel += 1;// *world.dangerLevelMod;
	
	unsigned pr = rand() % 100;
	if (pr < m_dangerLevel)
		setBattle(world); // начинается бой
}
//-----------------------------------------------------------------------------
void GameEnvironment::EndBattle()
{
	/*бой закончился, а тут будут проставляться какие-нибудь важности - типа таймера (ведь бой длился какое-то время), состояния мира от результатов боя и т.д.*/
	m_dangerLevel = 0;
	m_status = GameStatus::Exploring;
}
//-----------------------------------------------------------------------------
void GameEnvironment::setBattle(const World& world)
{
	m_status = GameStatus::Battle;
}
//-----------------------------------------------------------------------------