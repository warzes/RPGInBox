#pragma once

#include "Tile.h"

constexpr size_t MapSize = 50;

class Map final : NonCopyable, NonMovable
{
public:
	Map(ResourceManager& resources) noexcept;

	bool InitTest() noexcept;

	void Draw(IGameCamera* camera) noexcept;

	Tile tiles[MapSize][MapSize];

private:
	ResourceManager& m_resources;
};
