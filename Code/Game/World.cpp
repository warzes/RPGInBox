#include "stdafx.h"
#include "World.h"
//-----------------------------------------------------------------------------
World::~World()
{

}
//-----------------------------------------------------------------------------
bool World::Init()
{
	playerParty.SetPosition({ 0,0 });
	openworld.InitTest();
	return true;
}
//-----------------------------------------------------------------------------
void World::Move(const Point2& pos) noexcept
{
	if (playerParty.Move(pos))
	{
		// TODO: move event
	}
}
//-----------------------------------------------------------------------------