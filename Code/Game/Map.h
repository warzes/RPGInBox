#pragma once

#include "Tile.h"

constexpr size_t MapSize = 50;

class Map final : NonCopyable, NonMovable
{
public:
	bool InitTest(ResourceManager& resources) noexcept;

	void Draw(IGameCamera* camera) noexcept;

	Tile tiles[MapSize][MapSize];

private:

};
