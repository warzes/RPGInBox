#include "stdafx.h"
#include "World.h"
//-----------------------------------------------------------------------------
World::~World()
{

}
//-----------------------------------------------------------------------------
bool World::Init()
{
	if (!m_sky.Init())
		return false;

	playerParty.SetPosition({ 0,0 });
	openworld.InitTest();

	return true;
}
//-----------------------------------------------------------------------------
void World::Update(float deltaTime) noexcept
{
	m_sky.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void World::Draw(IGameCamera* camera) noexcept
{
	m_sky.Draw(camera);
}
//-----------------------------------------------------------------------------
void World::Move(const Point2& pos) noexcept
{
	if (playerParty.Move(pos))
	{
		// TODO: move event
		environment.NextStep(*this);
	}
}
//-----------------------------------------------------------------------------