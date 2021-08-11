#pragma once

#include "FreeCamera.h"
#include "PlayerTurnCamera.h"

class GameFrame;

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
		
	std::unique_ptr<GameFrame> m_gameFrame = nullptr;

	ResourceManager m_resourceMgr;

	// camera
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera* m_currentCamera = nullptr;
	bool m_turnCamera = true;

	bool m_isEnd = true;
};