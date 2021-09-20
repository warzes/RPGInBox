#pragma once

#include "FreeCamera.h"
#include "PlayerTurnCamera.h"
#include "WorldAdventure.h"
#include "ResourceManager.h"

class Engine;
class GameFrame;

class OldGame final : NonCopyable, NonMovable
{
public:
	OldGame(Engine& engine) noexcept;
	~OldGame();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept { return m_isEnd; }

private:
	Engine& m_engine;

	std::unique_ptr<GameFrame> m_gameFrame = nullptr;

	ResourceManager m_resourceMgr;
	WorldAdventure* m_world = nullptr;

	// camera
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;

	bool m_isEnd = true;
};