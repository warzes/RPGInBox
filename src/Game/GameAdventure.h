#pragma once

#include "FreeCamera.h"
#include "PlayerTurnCamera.h"

class GameApp;
class ResourceManager;
class WorldAdventure;

class GameAdventure final
{
public:
	GameAdventure(GameApp& gameApp, ResourceManager& m_resourceMgr) noexcept;
	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	GameApp& m_gameApp;
	ResourceManager& m_resourceMgr;
	WorldAdventure* m_world = nullptr;

	// camera
	FreeCamera m_freeCamera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;
};