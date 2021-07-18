﻿#include "stdafx.h"
#include "World.h"
World::World()
	: m_battleEngine(playerParty)
{
}
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
	if (environment.GetStatus() == GameStatus::Exploring)
	{
		if (m_battle) m_battleEngine.StopBattle();
		m_battle = false;
		m_sky.Update(deltaTime); // небо  не анимируется в бою (типа замедление времени)
	}
	else if (environment.GetStatus() == GameStatus::Battle)
	{
		if (!m_battle) // битва начинается
		{
			m_battle = true;
			m_battleEngine.StartBattle(getTestEnemy());
		}
		else
		{
			m_battleEngine.Update(deltaTime);
		}
	}		
}
//-----------------------------------------------------------------------------
void World::Draw(ResourceManager& resources, IGameCamera* camera) noexcept
{
	openworld.Draw(resources, camera);
	m_sky.Draw(camera);

	if (environment.GetStatus() == GameStatus::Exploring)
	{
		// Text
		{
			auto pos = playerParty.GetPosition();
			const std::string text = "Player pos {x=" + std::to_string(pos.x) + " y=" + std::to_string(pos.y) + "}";
			DrawText(text.c_str(), 10, 30, 20, WHITE);
		}
	}
	else if (environment.GetStatus() == GameStatus::Battle)
	{
		if (m_battle)
			m_battleEngine.Draw();
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
EnemyParty World::getTestEnemy()
{
	EnemyParty enemys;

	{
		Enemy enemy;
		enemys.enemys.emplace_back(enemy);
	}

	{
		Enemy enemy;
		enemys.enemys.emplace_back(enemy);
	}

	{
		Enemy enemy;
		enemys.enemys.emplace_back(enemy);
	}

	{
		Enemy enemy;
		enemys.enemys.emplace_back(enemy);
	}

	return enemys;
}
//-----------------------------------------------------------------------------Ы