#pragma once

#include "FreeCamera.h"
#include "PlayerTurnCamera.h"

class GameStateManager;
class ResourceManager;
class World;

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
	World* m_world = nullptr;

	// camera
	FreeCamera m_freeCamera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;
};