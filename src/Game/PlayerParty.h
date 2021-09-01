#pragma once

#include "Hero.h"
#include "EngineMath.h"

class PlayerParty final : NonCopyable, NonMovable
{
public:
	PlayerParty();

	Point2 GetPosition() const noexcept { return m_position; }
	void SetPosition(const Point2& pos) noexcept { m_position = m_oldPosition = pos; }

	bool Move(const Point2& pos) noexcept;

	Hero characters[MaxNumCharacterInPlayerParty];
	Point2 positionCharactersInParty[MaxNumCharacterInPlayerParty];

	// statistics
	unsigned level = 1;
	unsigned experience = 0;

	// party talents

	// inventory
	unsigned gold = 0;
	// TODO:

private:
	Point2 m_position;
	Point2 m_oldPosition;
};