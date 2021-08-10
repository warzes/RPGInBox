#pragma once

#include "PlayerParty.h"
#include "Map.h"
#include "GameEnvironment.h"
#include "BattleEngine.h"

class World final : NonCopyable, NonMovable
{
public:
	World(ResourceManager& resources);
	~World();
	bool Init();

	void Update(float deltaTime) noexcept;
	void Draw(IGameCamera* camera) noexcept;

	void Move(const Point2& pos) noexcept;

	PlayerParty playerParty;
	Map openworld;
	GameEnvironment environment;
private:
	EnemyParty getTestEnemy();

	ResourceManager& m_resources;

	Sky m_sky;
	BattleEngine m_battleEngine;


	bool m_battle = false;
};