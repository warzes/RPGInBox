#include "stdafx.h"
#include "WorldAdventure.h"
#include "IGameCamera.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
WorldAdventure::WorldAdventure(ResourceManager& resources)
	: m_resources(resources)
{
}
//-----------------------------------------------------------------------------
bool WorldAdventure::Init()
{
	if (!m_sky.Init())
		return false;

	playerParty.SetPosition({ 0, 0 });
	if (!openworld.InitTest(m_resources))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void WorldAdventure::Update(float deltaTime) noexcept
{
	m_sky.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void WorldAdventure::Draw(IGameCamera* camera) noexcept
{
	// TODO: для эффективности можно сделать так - сейчас я рендерю в текстуру и стираю ее, но когда открываются окна, не надо стирать текстуру и рендерить ее - а сразу же ее использовать
	openworld.Draw(camera);
	m_sky.Draw(camera->GetCamera(), camera->GetCameraPosition());

	// Text
	{
		auto pos = playerParty.GetPosition();
		const std::string text = "Player pos {x=" + std::to_string(pos.x) + " y=" + std::to_string(pos.y) + "}";
		DrawText(text.c_str(), 10, 30, 20, WHITE);
	}
}
//-----------------------------------------------------------------------------
void WorldAdventure::Move(const Point2& pos) noexcept
{
	if (playerParty.Move(pos))
	{
	}
}