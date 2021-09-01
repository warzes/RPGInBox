#pragma once

class IGameCamera;
class ResourceManager;

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
	Tree2
};

class Tile final
{
public:
	static Tile Create(ResourceManager& resource, TileType type,  TileDecorType decor);

	bool IsBlocked() const { return decor != TileDecorType::None; }

	void Draw(IGameCamera* camera, const Vector2& pos);

	TileType type = TileType::Grass;
	TileDecorType decor = TileDecorType::None;

	std::shared_ptr<Texture2D> textureTile = nullptr;
	std::shared_ptr<Texture2D> textureDecor = nullptr;
};