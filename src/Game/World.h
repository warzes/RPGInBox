#pragma once

#include "PlayerParty.h"
#include "Map.h"
#include "BattleEngine.h"
#include "Sky.h"

class World final : NonCopyable, NonMovable
{
public:
	World(ResourceManager& resources);

	bool Init();

	void Update(float deltaTime) noexcept;
	void Draw(IGameCamera* camera) noexcept;

	void Move(const Point2& pos) noexcept;

	PlayerParty playerParty;
	Map openworld;
private:
	EnemyParty getTestEnemy(); // TODO: перенести в Map, так как зависит от карты

	ResourceManager& m_resources;

	Sky m_sky;
};