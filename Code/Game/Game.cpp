#include "stdafx.h"
#include "Game.h"
#include <Engine/DebugNew.h>
#include "GameFrame.h"
//-----------------------------------------------------------------------------
Game::Game(Engine& engine) noexcept
	: m_engine(engine)
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

	m_cameraTurn.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;
	if (m_turnCamera) m_camera.HideCursor = false;
	if (m_turnCamera) m_currentCamera = &m_cameraTurn;
	else m_currentCamera = &m_camera;


	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void Game::Update(float deltaTime) noexcept
{
}
//-----------------------------------------------------------------------------
void Game::Frame() noexcept
{
	m_gameFrame->BeginFrame();
	{

	}	
	m_gameFrame->EndFrame();
}
//-----------------------------------------------------------------------------