#include "stdafx.h"
#include "GameStateManager.h"
//-----------------------------------------------------------------------------
GameStateManager::GameStateManager(ResourceManager& resourceMgr) noexcept
	: m_adventureState(*this, resourceMgr)
	, m_battleState(*this, resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool GameStateManager::Init() noexcept
{
	if (!m_adventureState.Init())
		return false;
	if (!m_battleState.Init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void GameStateManager::Update(float deltaTime) noexcept
{
	if (m_state == GameState::Adventure)
	{
		m_adventureState.Update(deltaTime);
	}		
	else if (m_state == GameState::BeginBattle)
	{
		m_battleState.StartBattle();
		SetState(GameState::Battle);		
	}
	else if (m_state == GameState::Battle)
	{
		m_battleState.Update(deltaTime);
	}
}
//-----------------------------------------------------------------------------
void GameStateManager::Frame() noexcept
{
	m_adventureState.Frame(); // сцена приключения рисуется даже в других режимах
	if (m_state == GameState::Battle)
		m_battleState.Frame();
}
//-----------------------------------------------------------------------------
void GameStateManager::SetState(GameState state) noexcept
{
	m_state = state;
}
//-----------------------------------------------------------------------------