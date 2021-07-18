#pragma once

class IGameCamera;
class ResourceManager;

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

	void Draw(ResourceManager& resources, IGameCamera* camera, const Vector2& pos);

	TileType type = TileType::Grass;
	TileDecorType decor = TileDecorType::None;
};

class Map final : NonCopyable, NonMovable
{
public:
	bool InitTest() noexcept;

	void Draw(ResourceManager& resources, IGameCamera* camera) noexcept;

	Tile tiles[MapSize][MapSize];

private:

};
