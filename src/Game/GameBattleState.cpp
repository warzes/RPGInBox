#include "stdafx.h"
#include "GameBattleState.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameBattleState::GameBattleState(Player& player, ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_view(resourceMgr)
	, m_player(player)	
{
}
//-----------------------------------------------------------------------------
bool GameBattleState::Init() noexcept
{
	if (!m_view.Init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void GameBattleState::StartBattle(EnemyParty* enemies) noexcept
{
	m_enemies = enemies;
	m_view.ResetCells();
	m_rules.StartBattle(&m_player, m_enemies);

	m_playerMenu.AddElements(PlayerActionMainMenu);	
}
//-----------------------------------------------------------------------------
void GameBattleState::Update(float deltaTime) noexcept
{
	m_rules.Run();
	int i = m_playerMenu.Run();
	if (i >= 0) m_playerMenu.SetSelect(i);
	//auto p = selectCell();
	//if (p.x >= 0 && p.y >= 0)
	//{	
	//	m_view.ResetCells();
	//	m_view.SetStatusCell(p.x, p.y, BattleCellStatus::Yellow);
	//}
}
//-----------------------------------------------------------------------------
void GameBattleState::Frame() noexcept
{
	m_view.Frame(m_player, m_enemies);
	m_playerMenu.Draw();
}
//-----------------------------------------------------------------------------
Point2 GameBattleState::selectCell() noexcept // TODO: возможно перенести в класс правил
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