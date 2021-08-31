#include "stdafx.h"
#include "GameStateManager.h"
//-----------------------------------------------------------------------------
GameStateManager::GameStateManager(ResourceManager& resourceMgr) noexcept
	: m_adventureState(resourceMgr)
	, m_battleState(resourceMgr)
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
		m_adventureState.Update(deltaTime);
	else if (m_state == GameState::Battle)
		m_battleState.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void GameStateManager::Frame() noexcept
{
	m_adventureState.Frame(); // сцена приключения рисуется даже в других режимах
	if (m_state == GameState::Battle)
		m_battleState.Frame();
}
//-----------------------------------------------------------------------------