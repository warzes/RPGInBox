#include "stdafx.h"
#include "Map.h"
//-----------------------------------------------------------------------------
void Map::InitTest() noexcept
{
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			tiles[x][y].type = TileType::Grass;

			int t = rand() % 20;
			if (t < 2)
				tiles[x][y].decor = TileDecorType::Tree;

			t = rand() % 100;
			if (t < 2)
				tiles[x][y].decor = TileDecorType::Town;

			t = rand() % 100;
			if (t < 10)
				tiles[x][y].type = TileType::Road;
		}
	}
}
//-----------------------------------------------------------------------------