#pragma once

#include "GameBattleCore.h"
#include "GameBattleView.h"

class GameStateManager;
class ResourceManager;

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

	GameBattleView m_view;
};