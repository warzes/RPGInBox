#include "stdafx.h"
#include "GameBattle.h"

GameBattle::GameBattle(GameStateManager& stateManager, ResourceManager& resourceMgr) noexcept
	: m_stateManager(stateManager)
	, m_resourceMgr(resourceMgr)
{
}

bool GameBattle::Init() noexcept
{
	return true;
}

void GameBattle::Update(float deltaTime) noexcept
{
}

void GameBattle::Frame() noexcept
{
}
