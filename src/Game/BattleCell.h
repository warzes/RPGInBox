#pragma once

#include "UIAnimSwords.h"
#include "BattleTargetMeleeAttack.h"
#include "BattleTargetRangeAttack.h"


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

struct CreatureInCell final
{
	ICreature* creature = nullptr;
	Point2 position;
};

class BattleCell final : NonCopyable, NonMovable
{
	friend class BattleCells;
public:
	void ResetCellStatus() noexcept;
	void Draw(float deltaTime, const Point2& pos, bool isAnimSwords, UIAnimSwords &animSwords) noexcept;

	void SetStatus(BattleCellStatus status) { m_status = status; }
	BattleCellStatus GetStatus() const { return m_status; }
	CreatureInCell* GetCreature() { return &m_creature; }
private:
	BattleCellStatus m_status = BattleCellStatus::Normal;
	CreatureInCell m_creature;
};

class BattleCells final : NonCopyable, NonMovable
{
public:
	bool Init(ResourceManager& resourceMgr) noexcept;

	void SetCreature(Player* player, EnemyParty* enemy) noexcept;

	void SetFirstMember() noexcept;
	int NextMembers() noexcept;

	void ViewCurrentMember() noexcept;

	CreatureInCell& GetCurrentMember() noexcept;
	CreatureInCell* GetSelectMember() noexcept;

	void ResetStatusCells() noexcept;
	void SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;

	void Draw(float deltaTime) noexcept;

	bool IsFinalAnimSworld() noexcept;

	bool IsMeleeAttack() noexcept; // может ли персонаж бить ближней атакой
	bool IsRangeAttack() noexcept; // может ли персонаж бить дальней атакой

	int GetNumberHero() const noexcept;
	int GetNumberEnemy() const noexcept;

	TargetMeleeAttack GetTargetMeleeAttack() noexcept;
	TargetRangeAttack GetTargetRangeAttack() noexcept;

	void ResetAnimSword() noexcept;

private:

	// при этом первые два ряда - противника, вторые - героев.
	BattleCell m_cells[3][4];
	std::vector<CreatureInCell*> m_members;
	unsigned m_currentMember = 0;

	UIAnimSwords m_animSwords;
	bool m_isAnimSwords = false;
};