#pragma once

#include "GameBattleView.h"
#include "Creature.h"

class ResourceManager;

class GameBattle final
{
public:
	GameBattle(Player& player, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle(const EnemyParty& enemys) noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	ResourceManager& m_resourceMgr;
	GameBattleView m_view;
	Player& m_player;
	EnemyParty m_enemys;
};