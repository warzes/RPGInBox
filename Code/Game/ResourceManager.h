#pragma once

class ResourceManager final : NonCopyable, NonMovable
{
public:
	~ResourceManager();

	bool Init();

	Texture2D textureGrass;
	Texture2D textureRoad;

	Texture2D textureTree;
	Texture2D textureTown;
};