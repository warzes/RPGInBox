#include "stdafx.h"
#include "GameBattle.h"
#include "DebugNew.h"

/*
* Начало боя
*	Вызывается метод StartBattle, в который передается группа игрока в виде ссылки и копия группы врага
* 
* BattleDraw
*	сделать координаты всех элементов
*/



//-----------------------------------------------------------------------------
GameBattle::GameBattle(GameStateManager& stateManager, ResourceManager& resourceMgr) noexcept
	: m_stateManager(stateManager)
	, m_resourceMgr(resourceMgr)
	, m_view(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool GameBattle::Init() noexcept
{
	if (!m_view.Init())
		return false;

	return true;
}
void GameBattle::StartBattle() noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattle::Update(float deltaTime) noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattle::Frame() noexcept
{
	m_view.Frame();
}
//-----------------------------------------------------------------------------