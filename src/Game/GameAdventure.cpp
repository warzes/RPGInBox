#include "stdafx.h"
#include "GameAdventure.h"
#include "WorldAdventure.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameAdventure::GameAdventure(ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_world(new WorldAdventure(resourceMgr))
{
}
//-----------------------------------------------------------------------------
bool GameAdventure::Init() noexcept
{
	m_cameraTurn.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_freeCamera.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_freeCamera.MoveSpeed.z = 10.0f;
	m_freeCamera.MoveSpeed.x = 5.0f;
	if (m_turnCamera) m_freeCamera.HideCursor = false;
	if (m_turnCamera) m_currentCamera = &m_cameraTurn;
	else m_currentCamera = &m_freeCamera;

	if (!m_world->Init())
		return false;

	auto playerPos = m_world->playerParty.GetPosition();
	m_cameraTurn.SetCameraPosition({ (float)playerPos.x, 0.0f, (float)playerPos.y }, 0.0f);

	return true;
}
//-----------------------------------------------------------------------------
void GameAdventure::Update(float deltaTime) noexcept
{
	m_world->Update(deltaTime);
	m_currentCamera->Update(*m_world); // TODO: избавить камеру от World. Из камеры достается новая позиция камеры, проверяется, можно ли туда идти, и затем устанавливается целью камеры
	if (!m_cameraTurn.IsProcessMoving())
	{
		auto const& camPos = m_currentCamera->GetCameraPosition();
		m_world->Move({ (int)camPos.x, (int)camPos.z });
	}
}
//-----------------------------------------------------------------------------
void GameAdventure::Frame() noexcept
{
	m_world->Draw(m_currentCamera);
}
//-----------------------------------------------------------------------------