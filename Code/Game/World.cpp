#include "stdafx.h"
#include "World.h"
//-----------------------------------------------------------------------------
bool Party::Move(const Vector3& pos) noexcept
{
	m_position.x = (float)pos.x; 
	m_position.y = (float)pos.z; 
	printf("PP = %d %d\n", m_position.x, m_position.y);

	if (m_oldPosition.x != m_position.x || m_oldPosition.y != m_position.y)
	{
		m_oldPosition = m_position;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
void Map::InitTest() noexcept
{
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
			int t = rand() % 20;
			if (t < 2)
				tiles[x][y].isTree = true;
		}
	}
}
//-----------------------------------------------------------------------------
World::~World()
{

}
//-----------------------------------------------------------------------------
bool World::Init()
{
	playerParty.SetPosition(Point2{ 0,0 });
	openworld.InitTest();
	return true;
}
//-----------------------------------------------------------------------------
void World::Move(const Vector3& pos) noexcept
{
	if (playerParty.Move(pos))
	{
		// TODO: move event
	}
}
//-----------------------------------------------------------------------------