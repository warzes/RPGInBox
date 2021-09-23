#pragma once

#include "GameBattleView.h"
#include "GameBattleRules.h"
#include "GameBattlePlayerMenu.h"

class ResourceManager;

class GameBattleState final
{
public:
	GameBattleState(Player& player, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle(EnemyParty* enemies) noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	Point2 selectCell() noexcept;
	ResourceManager& m_resourceMgr;
	GameBattleView m_view;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;
	GameBattleRules m_rules;
	GameBattlePlayerMenu m_playerMenu;
};