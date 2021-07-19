#include "stdafx.h"
#include "BattleEngine.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(const EnemyParty& enemy)
{
	m_enemy = enemy;
	computInitiative();
}
//-----------------------------------------------------------------------------
void BattleEngine::StopBattle()
{
}
//-----------------------------------------------------------------------------
void BattleEngine::Draw()
{
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime)
{
}
//-----------------------------------------------------------------------------
void BattleEngine::computInitiative()
{
	m_numberMembers = MaxNumCharacterInPlayerParty; // TODO: но вообще надо считать только живых и способных действовать
	m_numberMembers += m_enemy.enemys.size();


	//пока без инициативы, по такому принципу - сначала действуют все персонажи игрока с первого по последний, потом действуют все враги
	//а возможно такую механику и оставить надолго
}
//-----------------------------------------------------------------------------