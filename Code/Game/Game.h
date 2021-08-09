#pragma once

#include "ResourceManager.h"
#include "FreeCamera.h"
#include "PlayerTurnCamera.h"
#include "World.h"

class Game final : NonCopyable, NonMovable
{
public:
	Game(Engine& engine) noexcept;
	~Game();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept { return m_isEnd; }

private:
	Engine& m_engine;

	class GameFrame;
	GameFrame* m_gameFrame = nullptr;

	ResourceManager m_resourceMgr;
	World *m_world = nullptr;

	// camera
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;

	bool m_isEnd = true;
};