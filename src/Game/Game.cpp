#include "stdafx.h"
#include "Game.h"
#include "GameFrame.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
Game::Game() noexcept
	: m_gameState(m_resourceMgr)
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
}
//-----------------------------------------------------------------------------
bool Game::Init() noexcept
{
	m_gameFrame.reset(new GameFrame());

	if (!m_gameState.Init())
		return false;

	m_gameState.SetState(GameState::Battle);

	return true;
}
//-----------------------------------------------------------------------------
void Game::Update(float deltaTime) noexcept
{
	m_gameState.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void Game::Frame() noexcept
{
	m_gameFrame->BeginFrame();
	m_gameState.Frame();
	m_gameFrame->EndFrame();
}
//-----------------------------------------------------------------------------