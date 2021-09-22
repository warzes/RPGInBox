#include "stdafx.h"
#include "GameBattle.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameBattle::GameBattle(Player& player, ResourceManager& resourceMgr) noexcept
	: m_player(player)
	, m_resourceMgr(resourceMgr)
	, m_view(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool GameBattle::Init() noexcept
{
	if (!m_view.Init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void GameBattle::StartBattle(const EnemyParty& enemys) noexcept
{
	m_enemys = enemys;
}
//-----------------------------------------------------------------------------
void GameBattle::Update(float deltaTime) noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattle::Frame() noexcept
{
	m_view.Frame();
}
//-----------------------------------------------------------------------------