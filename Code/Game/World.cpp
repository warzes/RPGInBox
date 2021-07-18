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
	if (!openworld.InitTest())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void World::Update(float deltaTime) noexcept
{
	m_sky.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void World::Draw(ResourceManager& resources, IGameCamera* camera) noexcept
{
	openworld.Draw(resources, camera);
	m_sky.Draw(camera);

	// Text
	{
		auto pos = playerParty.GetPosition();
		const std::string text = "Player pos {x=" + std::to_string(pos.x) + " y=" + std::to_string(pos.y) + "}";
		DrawText(text.c_str(), 10, 30, 20, WHITE);
	}
}
//-----------------------------------------------------------------------------
void World::Move(const Point2& pos) noexcept
{
	if (playerParty.Move(pos))
	{
		printf("step\n");
		// TODO: move event
		environment.NextStep(*this);
	}
}
//-----------------------------------------------------------------------------