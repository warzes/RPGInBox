#include "stdafx.h"
#include "BattleCell.h"
#include "DebugNew.h"
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };
//-----------------------------------------------------------------------------
void BattleCell::ResetCellStatus() noexcept
{
	m_status = BattleCellStatus::Normal;
}
//-----------------------------------------------------------------------------
void BattleCell::Draw(const Point2& pos) const noexcept
{
}
//-----------------------------------------------------------------------------
void BattleCells::ResetCells() noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			m_cells[x][y].ResetCellStatus();
		}
	}
}
//-----------------------------------------------------------------------------
void BattleCells::Draw() const noexcept
{
	Point2 cellPos;
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			cellPos.x = LeftTopCoordCells.x + (SizeCoordCells.x + OffsetCoordCells.x) * (int)x;
			cellPos.y = LeftTopCoordCells.y + 5 + (SizeCoordCells.y + OffsetCoordCells.y) * (int)y;
			m_cells[x][y].Draw();
		}
	}
}
//-----------------------------------------------------------------------------