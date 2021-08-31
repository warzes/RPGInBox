#pragma once

#include "FreeCamera.h"
#include "PlayerTurnCamera.h"
#include "World.h"

class GameStateManager;

class GameAdventure final
{
public:
	GameAdventure(GameStateManager& stateManager, ResourceManager& m_resourceMgr) noexcept;
	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	GameStateManager& m_stateManager;
	ResourceManager& m_resourceMgr;

	// camera
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;

	World* m_world = nullptr;
};