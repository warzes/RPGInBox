#include "stdafx.h"
#include "Map.h"
#include "IGameCamera.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
bool Map::InitTest(ResourceManager& resources) noexcept
{
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			TileType type = TileType::Grass;
			TileDecorType decor = TileDecorType::None;

			int t = rand() % 100;
			if (t < 10) type = TileType::Road;
			t = rand() % 20;
			if (t < 2) decor = TileDecorType::Tree;
			t = rand() % 40;
			if (t < 2) decor = TileDecorType::Tree2;

			tiles[x][y] = Tile::Create(resources, type, decor);
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
void Map::Draw(IGameCamera* camera) noexcept
{
	BeginMode3D(camera->GetCamera());
	camera->ExtractFrustum();

	//�� ����� ���������� ��� �����, � ���� ���� ����������� ������, ��� ������� �� ������ � � ����� ���� (����� ���� ��������� ���� ����� �� ������� ������ �� ����� ����� ��������� - ���� ����� � �����)
	//	������� ����� �������� ��� ��������� ������
	// 
	// ����� - ���� � ���� �������� ������� �������, �� ���� �������� �� ���� �����, ����� ������� �������

	for (unsigned x = 0; x < MapSize; x++)
	{
		for (unsigned y = 0; y < MapSize; y++)
		{
			const Vector3 min = { (float)x - 0.5f, 0.0f, (float)y - 0.5f };
			const Vector3 max = { (float)x + 0.5f, 2.0f, (float)y + 0.5f };

			if (camera->GetFrustum().AABBoxIn(min, max))
			{
				tiles[x][y].Draw(camera, { (float)x, (float)y });
			}
		}
	}

	EndMode3D();
}
//-----------------------------------------------------------------------------