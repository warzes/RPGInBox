#pragma once

class ICreature;

enum class BattleCellStatus
{
	Normal,	// ничего не дорисовывает
	Yellow,	// выделяет желтым цветом (например текущий персонаж)
	Green,	// выделяет зеленым цветом (например возможные клетки для выбора)
	Red,	// выделяет красным цветом (например недоступные клетки для выбора)
	Blue,	// выделяет красным цветом (например текущий выбор)
	Grey	// выделяет серым цветом
};


class BattleCell final
{
public:
	void ResetCellStatus() noexcept;
	void Draw(const Point2& pos) const noexcept;

private:
	BattleCellStatus m_status = BattleCellStatus::Normal;
	ICreature* m_creature = nullptr;
};

class BattleCells final
{
public:
	void ResetCells() noexcept;
	void Draw() const noexcept;

private:
	BattleCell m_cells[3][4];
};