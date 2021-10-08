#pragma once

class ICreature;

enum class BattleCellStatus
{
	Normal,	// ������ �� ������������
	Yellow,	// �������� ������ ������ (�������� ������� ��������)
	Green,	// �������� ������� ������ (�������� ��������� ������ ��� ������)
	Red,	// �������� ������� ������ (�������� ����������� ������ ��� ������)
	Blue,	// �������� ������� ������ (�������� ������� �����)
	Grey	// �������� ����� ������
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