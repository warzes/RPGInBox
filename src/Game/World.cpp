#include "stdafx.h"
#include "World.h"
#include "IGameCamera.h"
#include "EnemyTemplateData.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
World::World(ResourceManager& resources)
	: m_resources(resources)
	, m_battleEngine(playerParty)
{
}
//-----------------------------------------------------------------------------
bool World::Init()
{
	if (!m_sky.Init())
		return false;

	playerParty.SetPosition({ 0, 0 });
	if (!openworld.InitTest(m_resources))
		return false;

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
void World::Move(const Point2& pos) noexcept
{
	if (playerParty.Move(pos))
	{
		// TODO: move event
		environment.NextStep(*this);
	}
}
//-----------------------------------------------------------------------------
EnemyParty World::getTestEnemy()
{
	EnemyParty enemys;

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 0, 0 });
	}

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 1, 0 });
	}

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 2, 0 });
	}

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 0, 1 });
	}

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 1, 1 });
	}

	{
		enemys.enemys.emplace_back(EnemyTemplateData::CreateTest());
		enemys.positionCharactersInParty.push_back({ 2, 1 });
	}

	return enemys;
}
//-----------------------------------------------------------------------------