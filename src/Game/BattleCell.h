#pragma once

#include "UIAnimSwords.h"

class ICreature;
class Player;
class EnemyParty;
class ResourceManager;

enum class BattleCellStatus
{
	Normal,	// ничего не дорисовывает
	Yellow,	// выделяет желтым цветом (например текущий персонаж)
	Green,	// выделяет зеленым цветом (например возможные клетки для выбора)
	Red,	// выделяет красным цветом (например недоступные клетки для выбора)
	Blue,	// выделяет красным цветом (например текущий выбор)
	Grey	// выделяет серым цветом
};

struct CreatureInCell
{
	ICreature* creature = nullptr;
	Point2 posInCell;
};

class BattleCell final
{
	friend class BattleCells;
public:
	void ResetCellStatus() noexcept;
	void Draw(float deltaTime, const Point2& pos, bool isAnimSwords, UIAnimSwords &animSwords) const noexcept;

private:
	BattleCellStatus m_status = BattleCellStatus::Normal;
	CreatureInCell m_creature;
};

class BattleCells final
{
public:
	bool Init(ResourceManager& resourceMgr) noexcept;

	void SetCreature(Player* player, EnemyParty* enemy) noexcept;

	void ResetCells() noexcept;
	void SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;

	void Draw(float deltaTime) noexcept;

	void SetFirstMember() noexcept;
	int NextMembers() noexcept;

	CreatureInCell& GetCurrentMember() noexcept;

	bool IsFinalAnimSworld() noexcept;

	bool ViewMeleeAttack(int selectTargetCell) noexcept;

	void ViewSelectMember() noexcept;


	bool IsMeleeAttack() const noexcept; // может ли персонаж бить ближней атакой
	bool IsRangeAttack() const noexcept; // может ли персонаж бить дальней атакой

	int GetHero() const noexcept;
	int GetEnemy() const noexcept;

private:
	void resetAnimSword() noexcept;

	// при этом первые два ряда - противника, вторые - героев.
	BattleCell m_cells[3][4];
	std::vector<CreatureInCell*> m_members;
	unsigned m_currentMember = 0;

	UIAnimSwords m_animSwords;
	bool m_isAnimSwords = false;
};