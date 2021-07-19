#include "stdafx.h"
#include "BattleEngine.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(ResourceManager& resources, const EnemyParty& enemy)
{
	m_enemy = enemy;
	m_patchTexture = resources.GetTexture("../resources/ninepatch.png");

 	//пока без инициативы, по такому принципу - сначала действуют все персонажи игрока с первого по последний, потом действуют все враги
	//а возможно такую механику и оставить надолго
	for (size_t i = 0; i < MaxNumCharacterInPlayerParty; i++)
	{
		member m;
		m.type = member::type_::player;
		m.number = i;
		m_members.push_back(m);
	}
	for (size_t i = 0; i < m_enemy.enemys.size(); i++)
	{
		member m;
		m.type = member::type_::enemy;
		m.number = i;
		m_members.push_back(m);
	}
	m_currentMember = m_members.size();
}
//-----------------------------------------------------------------------------
void BattleEngine::StopBattle()
{
}
//-----------------------------------------------------------------------------
void BattleEngine::Draw()
{
	if (!m_patchTexture) return;

	m_dest.width = (float)GetScreenWidth() - m_dest.x * 2.0f;
	m_dest.height = (float)GetScreenHeight() - m_dest.y * 2.0f;

    DrawTextureNPatch(*m_patchTexture, m_ninePatchInfo, m_dest, { 0.0f, 0.0f }, 0.0f, WHITE);

	DrawText("Test text", 20, 20, 20, WHITE);
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime)
{
	// все участники участвовали (или бой только начался)
	if (m_currentMember == m_members.size())
		nextRound();

	// обработка текущего раунда
	currentRound();

	// TODO: ивент завершения боя по состоянию участников боя
}
//-----------------------------------------------------------------------------
void BattleEngine::nextRound()
{
	m_currentMember = 0;
}
//-----------------------------------------------------------------------------
void BattleEngine::currentRound()
{
	bool nextMember = false;
	if (m_members[m_currentMember].type == member::type_::player)
		nextMember = playerAction();
	else if (m_members[m_currentMember].type == member::type_::enemy)
		nextMember = enemyAction();

	if (nextMember) // текущий участник подействовал, переключаемся на следующего
		m_currentMember++;
}
//-----------------------------------------------------------------------------
bool BattleEngine::playerAction()
{
	return false;
}
//-----------------------------------------------------------------------------
bool BattleEngine::enemyAction()
{
	return false;
}
//-----------------------------------------------------------------------------