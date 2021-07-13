#pragma once

constexpr size_t MapSize = 50;

enum class TileType
{
	// outdoor
	Grass,
	Road
};

enum class TileDecorType
{
	None,
	Tree,
	Town
};

class Tile final : NonCopyable, NonMovable
{
public:
	bool IsBlocked() const { return decor != TileDecorType::None; }

	TileType type = TileType::Grass;
	TileDecorType decor = TileDecorType::None;

};

class Map final : NonCopyable, NonMovable
{
public:
	void InitTest() noexcept;

	Tile tiles[MapSize][MapSize];
};
