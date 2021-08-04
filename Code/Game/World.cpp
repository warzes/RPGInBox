#include "stdafx.h"
#include "World.h"
World::World(ResourceManager& resources)
	: m_resources(resources)
	, m_battleEngine(playerParty)
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
	if (!openworld.InitTest(m_resources))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void World::Update(float deltaTime) noexcept
{
	if (environment.GetStatus() == GameStatus::Exploring)
	{
		m_battle = false;
		m_sky.Update(deltaTime); // небо  не анимируется в бою (типа замедление времени)
	}
	else if (environment.GetStatus() == GameStatus::Battle)
	{
		if (!m_battle) // битва начинается
		{
			m_battle = true;
			m_battleEngine.StartBattle(m_resources, getTestEnemy());
		}
		else
		{
			m_battleEngine.Update(deltaTime);
		}
	}		
}
//-----------------------------------------------------------------------------
void World::Draw(IGameCamera* camera) noexcept
{
	// TODO: для эффективности можно сделать так - сейчас я рендерю в текстуру и стираю ее, но когда открываются окна, не надо стирать текстуру и рендерить ее - а сразу же ее использовать
	openworld.Draw(camera);
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
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 0, 0 });
	}

	{
		Enemy enemy;
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 1, 0 });
	}

	{
		Enemy enemy;
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 2, 0 });
	}

	{
		Enemy enemy;
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 0, 1 });
	}

	{
		Enemy enemy;
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 1, 1 });
	}

	{
		Enemy enemy;
		enemy.statistics.hp = 10;
		enemys.enemys.emplace_back(enemy);
		enemys.positionCharactersInParty.push_back({ 2, 1 });
	}

	return enemys;
}
//-----------------------------------------------------------------------------Ы