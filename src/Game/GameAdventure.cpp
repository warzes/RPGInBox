#include "stdafx.h"
#include "GameAdventure.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameAdventure::GameAdventure(GameStateManager& stateManager, ResourceManager& resourceMgr) noexcept
	: m_stateManager(stateManager)
	, m_resourceMgr(resourceMgr)
	, m_world(new World(resourceMgr))
{
}
//-----------------------------------------------------------------------------
bool GameAdventure::Init() noexcept
{
	m_cameraTurn.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;
	if (m_turnCamera) m_camera.HideCursor = false;
	if (m_turnCamera) m_currentCamera = &m_cameraTurn;
	else m_currentCamera = &m_camera;

	if (!m_world || !m_world->Init())
		return false;

	auto playerPos = m_world->playerParty.GetPosition();
	m_cameraTurn.SetCameraPosition({ (float)playerPos.x, 0.0f, (float)playerPos.y }, 0.0f);

	return true;
}
//-----------------------------------------------------------------------------
void GameAdventure::Update(float deltaTime) noexcept
{
	m_world->Update(deltaTime);
	// TODO: возможно логику камеру также перенести в m_world?
	if (m_world->environment.GetStatus() == GameStatus::Exploring)
	{
		m_currentCamera->Update(*m_world);
		if (!m_cameraTurn.IsProcessMoving())
		{
			auto const& camPos = m_currentCamera->GetCameraPosition();
			m_world->Move({ (int)camPos.x, (int)camPos.z });
		}
	}
}
//-----------------------------------------------------------------------------
void GameAdventure::Frame() noexcept
{
	m_world->Draw(m_currentCamera);
}
//-----------------------------------------------------------------------------