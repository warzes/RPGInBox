﻿#pragma once

#include "PlayerParty.h"
#include "Map.h"

#include "Sky.h"

class WorldAdventure final : NonCopyable, NonMovable
{
public:
	WorldAdventure(ResourceManager& resources);

	bool Init();

	void Update(float deltaTime) noexcept;
	void Draw(IGameCamera* camera) noexcept;

	void Move(const Point2& pos) noexcept;

	PlayerParty playerParty;
	Map openworld;
private:

	ResourceManager& m_resources;

	Sky m_sky;
};