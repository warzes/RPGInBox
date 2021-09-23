#include "stdafx.h"
#include "GameBattleRules.h"
#include "Creature.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void GameBattleRules::StartBattle(Player* player, EnemyParty* enemies) noexcept
{
	m_round = 0;
	m_state = BattleState::NewRound;

	m_player = player;
	m_enemies = enemies;
	m_members.clear();
	for (size_t i = 0; i < m_player->heroes.size(); i++)
		m_members.push_back(&m_player->heroes[i]);
	for (size_t i = 0; i < enemies->enemys.size(); i++)
		m_members.push_back(&enemies->enemys[i]);
	m_currentMember = 0;
}
//-----------------------------------------------------------------------------
void GameBattleRules::Run() noexcept
{
	if (m_state == BattleState::NewRound)
	{
		newRound();
	}
	else if (m_state == BattleState::SelectAction)
	{
		// TODO: игрок может управлять не только своими существами, а например очарованными. Или наоборот некоторые его герои будут неконтролируемыми
		bool isPlayer = false;
		if (m_members[m_currentMember]->GetType() == CreatureType::Hero)
			selectPlayerAction();
		else
			selectEnemyAction();
	}
}
//-----------------------------------------------------------------------------
void GameBattleRules::newRound() noexcept
{
	m_currentMember = 0; // первый участник боя в новом раунде начинает
	m_state = BattleState::SelectAction; // ожидание выбора действия
}
//-----------------------------------------------------------------------------
void GameBattleRules::selectPlayerAction() noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattleRules::selectEnemyAction() noexcept
{
}
//-----------------------------------------------------------------------------