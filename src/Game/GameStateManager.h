#pragma once

#include "GameAdventure.h"
#include "GameBattle.h"

enum class GameState
{
	Adventure,
	Battle
};

class GameStateManager final
{
public:
	GameStateManager(ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	void SetState(GameState state) noexcept;

private:
	GameState m_state = GameState::Adventure;
	GameAdventure m_adventureState;
	GameBattle m_battleState;
};