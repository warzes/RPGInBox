#include "stdafx.h"
#include "Game.h"
#include "GameScenes.h"
#include "Sound.h"
#include "Global.h"
#include "Player.h"
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
	SoundManager::Close();
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

	if (!SoundManager::Init())
		return false;

	if (!Global::Init())
		return false;
	if (!Player::Init())
		return false;

	GameScenes::SwitchScene(SceneId_Intro);
	GameScenes::PerformSceneChange();

	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void Game::Update(float deltaTime) noexcept
{
	SoundManager::Update();
	GameScenes::Update();
}
//-----------------------------------------------------------------------------
void Game::Frame() noexcept
{
	m_gameFrame->BeginFrame();
	{
		GameScenes::Draw();
	}	
	m_gameFrame->EndFrame();
}
//-----------------------------------------------------------------------------