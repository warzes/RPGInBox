#include "stdafx.h"
#include "GameBattle.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameBattle::GameBattle(GameStateManager& stateManager, ResourceManager& resourceMgr) noexcept
	: m_stateManager(stateManager)
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
void GameBattle::Update(float deltaTime) noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattle::Frame() noexcept
{
	m_view.Frame();
}
//-----------------------------------------------------------------------------