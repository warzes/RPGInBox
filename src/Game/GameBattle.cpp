#include "stdafx.h"
#include "GameBattle.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameBattle::GameBattle(Player& player, ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_view(resourceMgr)
	, m_player(player)	
{
}
//-----------------------------------------------------------------------------
bool GameBattle::Init() noexcept
{
	if (!m_view.Init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void GameBattle::StartBattle(EnemyParty* enemies) noexcept
{
	m_enemies = enemies;
	m_view.ResetCells();
}
//-----------------------------------------------------------------------------
void GameBattle::Update(float deltaTime) noexcept
{
	auto p = selectCell();
	if (p.x >= 0 && p.y >= 0)
	{	
		m_view.ResetCells();
		m_view.SetStatusCell(p.x, p.y, BattleCellStatus::Yellow);
	}
}
//-----------------------------------------------------------------------------
void GameBattle::Frame() noexcept
{
	m_view.Frame(m_player, m_enemies);
}
//-----------------------------------------------------------------------------
Point2 GameBattle::selectCell() noexcept
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		auto pos = GetMousePosition();

		pos.x -= LeftTopCoordCells.x;
		pos.y -= LeftTopCoordCells.y;
		if (pos.x > 0 && pos.y > 0)
		{
			Point2 p;
			p.x = (int)pos.x / (SizeCoordCells.x + OffsetCoordCells.x);
			p.y = (int)pos.y / (SizeCoordCells.y + OffsetCoordCells.y);
			if (p.x < 3 && p.y < 4)
				return p;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		m_view.ResetCells();
	}
	return {-1,-1};
}
//-----------------------------------------------------------------------------