#pragma once

class BattleCell;

// класс хранящий массив целей для ближней атаки - только один ряд
class TargetMeleeAttack final
{
public:
	TargetMeleeAttack() noexcept;

	bool IsZero() const noexcept;

	void SetPos(int x) noexcept;
	void Update() noexcept;

	std::vector<BattleCell*> target;
	int selectPos = 0;

private:
	int findX(int mod) noexcept;
};