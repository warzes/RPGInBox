#pragma once

class GameStateManager;

class GameBattle final
{
public:
	GameBattle(GameStateManager& stateManager, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	GameStateManager& m_stateManager;
	ResourceManager& m_resourceMgr;
};