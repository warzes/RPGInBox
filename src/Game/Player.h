#pragma once

#include "Creature.h"
#include "EngineMath.h"

// нет отдельного класса под партию - не нужен, передавать сразу игрока
class Player final : NonCopyable, NonMovable
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr) noexcept;

	Point2 GetPosition() const noexcept { return m_position; }
	void SetPosition(const Point2& pos) noexcept { m_position = m_oldPosition = pos; }

	bool Move(const Point2& pos) noexcept;

	// statistics
	unsigned level = 1;
	unsigned experience = 0;

	// party talents

	// inventory
	unsigned gold = 0;

	Hero* grid[3][2] = { nullptr }; // расположение героев. [0-2][0] - первый ряд; [0-2][1] - второй ряд
	std::vector<Hero> heroes;

private:
	Point2 m_position;
	Point2 m_oldPosition;
};