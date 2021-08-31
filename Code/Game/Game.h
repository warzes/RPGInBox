#pragma once

#include "GameStateManager.h"

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
	GameStateManager m_gameState;
	bool m_isEnd = true;
};