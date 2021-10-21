#pragma once

class BattleCell;

// класс хранящий массив целей для дальней атаки
class TargetRangeAttack final
{
public:
	TargetRangeAttack() noexcept;

	bool IsZero() const noexcept;

	void SetPos(int x, int y) noexcept;
	void Update() noexcept;

	std::vector<std::vector<BattleCell*>> target;
	Point2 selectPos;

private:
	int findX(int mod) noexcept;
	int findY(int mod) noexcept;
};