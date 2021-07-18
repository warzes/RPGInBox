#include "stdafx.h"
#include "Map.h"
#include "IGameCamera.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
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

	// floor - grass
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
			const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

			if (tiles[x][y].type == TileType::Grass /* && m_camera.GetFrustum().AABBoxIn(min, max)*/)
			{
				DrawCubeTexture(resources.textureGrass, Vector3{ (float)x, -0.5f, (float)y }, 1, 1, 1, WHITE);
				//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
			}
		}
	}

	// floor - road
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
			const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

			if (tiles[x][y].type == TileType::Road && camera->GetFrustum().AABBoxIn(min, max))
			{
				DrawCubeTexture(resources.textureRoad, Vector3{ (float)x, -0.5f, (float)y }, 1, 1, 1, WHITE);
				//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
			}
		}
	}

	// tree
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
			const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

			if (tiles[x][y].decor == TileDecorType::Tree)
			{
				if (camera->GetFrustum().AABBoxIn(min, max))
				{
					DrawBillboard(camera->GetCamera(), resources.textureTree, Vector3{ (float)x, 1.0f, (float)y }, 2.0f, WHITE);
					//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
					//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
					//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
				}
			}
		}
	}

	// town
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
			const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

			if (tiles[x][y].decor == TileDecorType::Town)
			{
				if (camera->GetFrustum().AABBoxIn(min, max))
				{
					DrawBillboard(camera->GetCamera(), resources.textureTown, Vector3{ (float)x, 1.0f, (float)y }, 2.0f, WHITE);
					//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
					//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
					//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
				}
			}
		}
	}

	EndMode3D();
}
//-----------------------------------------------------------------------------