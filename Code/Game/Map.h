#pragma once

constexpr size_t MapSize = 20;

class Tile final : NonCopyable, NonMovable
{
public:
	bool isTree = false;
};

class Map final : NonCopyable, NonMovable
{
public:
	void InitTest() noexcept;

	Tile tiles[MapSize][MapSize];
};
