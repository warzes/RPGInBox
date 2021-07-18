#include "stdafx.h"
#include "Map.h"
#include "IGameCamera.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void Tile::Draw(ResourceManager& resources, IGameCamera* camera, const Vector2& pos)
{
	// Floor render	
	if (type == TileType::Grass )
	{
		DrawCubeTexture(resources.textureGrass, {pos.x, -0.5f, pos.y}, 1, 1, 1, WHITE);
		//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
	}
	else if (type == TileType::Road)
	{
		DrawCubeTexture(resources.textureRoad, { pos.x, -0.5f, pos.y }, 1, 1, 1, WHITE);
		//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
	}

	// object render

	if (decor == TileDecorType::Tree)
	{
		DrawBillboard(camera->GetCamera(), resources.textureTree, { pos.x, 1.0f, pos.y }, 2.0f, WHITE);
		//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
		//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
		//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
	}
	else if (decor == TileDecorType::Town)
	{
		DrawBillboard(camera->GetCamera(), resources.textureTown, { pos.x, 1.0f, pos.y }, 2.0f, WHITE);
			//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
			//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
			//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
	}
}
//-----------------------------------------------------------------------------
bool Map::InitTest() noexcept
{
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			tiles[x][y].type = TileType::Grass;

			int t = rand() % 20;
			if (t < 2)
				tiles[x][y].decor = TileDecorType::Tree;

			t = rand() % 40;
			if (t < 2)
				tiles[x][y].decor = TileDecorType::Town;

			t = rand() % 100;
			if (t < 10)
				tiles[x][y].type = TileType::Road;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
void Map::Draw(ResourceManager& resources, IGameCamera* camera) noexcept
{
	BeginMode3D(camera->GetCamera());
	camera->ExtractFrustum();

	//не нужно перебирать всю карту, у меня есть направление камеры, вот начиная от игрока и в глубь идти (можно даже прерывать если точно за текущим тайлом не будет видно остальных - типа стены в данже)
	//	перебор всего оставить для свободной камеры

	for (unsigned x = 0; x < MapSize; x++)
	{
		for (unsigned y = 0; y < MapSize; y++)
		{
			const Vector3 min = { (float)x - 0.5f, 0.0f, (float)y - 0.5f };
			const Vector3 max = { (float)x + 0.5f, 2.0f, (float)y + 0.5f };

			if (camera->GetFrustum().AABBoxIn(min, max))
			{
				tiles[x][y].Draw(resources, camera, { (float)x, (float)y });
			}

		}
	}

	EndMode3D();
}
//-----------------------------------------------------------------------------