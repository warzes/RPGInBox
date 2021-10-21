#include "stdafx.h"
#include "BattleTargetRangeAttack.h"
#include "BattleCell.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
TargetRangeAttack::TargetRangeAttack() noexcept
{
	target.resize(MaxNumberCellInBattle);
	for (int i = 0; i < MaxNumberCellInBattle; i++)
	{
		target[i].resize(MaxNumberHalfLineInBattle);
	}
	for (int y = 0; y < MaxNumberHalfLineInBattle; y++)
	{
		for (int x = 0; x < MaxNumberCellInBattle; x++)
		{
			target[x][y] = nullptr;
		}
	}
}
//-----------------------------------------------------------------------------
bool TargetRangeAttack::IsZero() const noexcept
{
	for (int y = 0; y < MaxNumberHalfLineInBattle; y++)
	{
		for (int x = 0; x < MaxNumberCellInBattle; x++)
		{
			if (target[x][y] != nullptr) return false;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
void TargetRangeAttack::SetPos(int x, int y) noexcept
{
#if 1
	int nearest_x = std::clamp(x, 0, (int)MaxNumberCellInBattle-1);
	int nearest_y = std::clamp(y, 0, (int)MaxNumberHalfLineInBattle-1);

	int min_distance = MaxNumberCellInBattle * MaxNumberCellInBattle + MaxNumberHalfLineInBattle * MaxNumberHalfLineInBattle;
	for (int _y = 0; _y < MaxNumberHalfLineInBattle; _y++)
	{
		for (int _x = 0; _x < MaxNumberCellInBattle; _x++)
		{
			int distance = (x - _x) * (x - _x) + (y - _y) * (y - _y);
			if (target[_x][_y] && min_distance > distance)
			{
				min_distance = distance;
				nearest_x = _x;
				nearest_y = _y;
			}
		}
	}

	selectPos.x = nearest_x;
	selectPos.y = nearest_y;
#else
	if (y >= MaxNumberHalfLineInBattle) y-= MaxNumberHalfLineInBattle;
	if (target[x][y] != nullptr)
	{
		selectPos.x = x;
		selectPos.y = y;
	}
	else
	{
		// TODO: улучшить - чтобы выбирался оптимальный
		for (int y = 0; y < MaxNumberHalfLineInBattle; y++)
		{
			for (int x = 0; x < MaxNumberCellInBattle; x++)
			{
				if (target[x][y] != nullptr)
				{
					selectPos.x = x;
					selectPos.y = y;
				}
			}
		}
	}
#endif
}
//-----------------------------------------------------------------------------
void TargetRangeAttack::Update() noexcept
{
	for (int y = 0; y < MaxNumberHalfLineInBattle; y++)
	{
		for (int x = 0; x < MaxNumberCellInBattle; x++)
		{
			if (target[x][y] != nullptr)
			{
				target[x][y]->SetStatus(BattleCellStatus::Green);
			}
		}
	}

	if (Input::IsPressed(GameKey::Left))
	{
		selectPos.x = findX(-1);
	}
	if (Input::IsPressed(GameKey::Right))
	{
		selectPos.x = findX(1);
	}
	if (Input::IsPressed(GameKey::Up))
	{
		selectPos.y = findY(-1);
	}
	if (Input::IsPressed(GameKey::Down))
	{
		selectPos.y = findY(1);
	}
	if (selectPos.x >= 0 && selectPos.x < MaxNumberCellInBattle && selectPos.y >= 0 && selectPos.y < MaxNumberHalfLineInBattle)
		target[selectPos.x][selectPos.y]->SetStatus(BattleCellStatus::Blue);
}
//-----------------------------------------------------------------------------
int TargetRangeAttack::findX(int mod) noexcept
{
	const int y = selectPos.y;
	int numX = 0;
	for (int x = 0; x < MaxNumberCellInBattle; x++)
	{
		if (target[x][y] != nullptr) numX++;
	}
	if (numX == 0) return -1;          // никого нет в этом ряду
	if (numX == 1) return selectPos.x; // некуда двигать, он один

	int x = selectPos.x;

	if (mod == -1)
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numX) break; // если ушло в бесконечный цикл
			if (x == 0) x = 2;
			else x--;
			if (target[x][y]) break;
		}
	}
	else
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numX) break; // если ушло в бесконечный цикл
			if (x == 2) x = 0;
			else x++;
			if (target[x][y]) break;
		}
	}

	return x;
}
//-----------------------------------------------------------------------------
int TargetRangeAttack::findY(int mod) noexcept
{
	const int x = selectPos.x;
	int numY = 0;
	for (int y = 0; y < MaxNumberHalfLineInBattle; y++)
	{
		if (target[x][y] != nullptr) numY++;
	}
	if (numY == 0) return -1;          // никого нет в этом ряду
	if (numY == 1) return selectPos.y; // некуда двигать, он один

	int y = selectPos.y;

	if (mod == -1)
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numY) break; // если ушло в бесконечный цикл
			if (y == 0) y = 1;
			else y--;
			if (target[x][y]) break;
		}
	}
	else
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numY) break; // если ушло в бесконечный цикл
			if (y == 1) y = 0;
			else y++;
			if (target[x][y]) break;
		}
	}

	return y;
}
//-----------------------------------------------------------------------------