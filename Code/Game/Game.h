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
	bool m_isEnd = true;

	ResourceManager m_resourceMgr;
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	World m_world;

	bool m_turnCamera = true;

	class GameFrame;
	GameFrame*m_gameFrame = nullptr;
};