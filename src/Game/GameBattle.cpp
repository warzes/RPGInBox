#include "stdafx.h"
#include "GameBattle.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameBattle::GameBattle(Player& player, ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_view(resourceMgr)
	, m_player(player)	
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
void GameBattle::StartBattle(const EnemyParty& enemies) noexcept
{
	m_enemies = enemies;
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