#pragma once

#include "GameAdventure.h"
#include "GameBattle.h"

class DataManager;

enum class GameState
{
	MainMenu,

	Adventure,

	//-------------------------------------------------------------------------
	// Battle state
	BeginBattle,
	Battle
};

class GameStateManager final
{
public:
	GameStateManager(ResourceManager& resourceMgr, DataManager& data) noexcept;
	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	void SetState(GameState state) noexcept;

	bool IsEnd() const noexcept { return m_isEnd; }
	void End() noexcept { m_isEnd = true; }

private:
	GameState m_state = GameState::Adventure;
	GameAdventure m_adventureState;
	GameBattle m_battleState;
	bool m_isEnd = false;
};