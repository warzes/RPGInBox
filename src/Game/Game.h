#pragma once

#include "GameStateManager.h"
#include "ResourceManager.h"

class GameFrame;

class Game final : NonCopyable, NonMovable
{
public:
	Game() noexcept;
	~Game();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept { return m_gameState.IsEnd(); }

private:
	std::unique_ptr<GameFrame> m_gameFrame = nullptr;
	ResourceManager m_resourceMgr;
	GameStateManager m_gameState;
};