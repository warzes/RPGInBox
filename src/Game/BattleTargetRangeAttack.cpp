#include "stdafx.h"
#include "BattleTargetRangeAttack.h"
#include "BattleCell.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
TargetRangeAttack::TargetRangeAttack() noexcept
{
	target.resize(3);
	for (int i = 0; i < 3; i++)
	{
		target[i].resize(2);
	}
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			target[x][y] = nullptr;
		}
	}
}
//-----------------------------------------------------------------------------
bool TargetRangeAttack::IsZero() const noexcept
{
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (target[x][y] != nullptr) return false;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
void TargetRangeAttack::SetPos(int x, int y) noexcept
{
	if (y >= 2) y-=2;
	if (target[x][y] != nullptr)
	{
		selectPos.x = x;
		selectPos.y = y;
	}
	else
	{
		// TODO: улучшить - чтобы выбирался оптимальный
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (target[x][y] != nullptr)
				{
					selectPos.x = x;
					selectPos.y = y;
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void TargetRangeAttack::Update() noexcept
{
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
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
	if (selectPos.x >= 0 && selectPos.x < 3 && selectPos.y >= 0 && selectPos.y < 2)
		target[selectPos.x][selectPos.y]->SetStatus(BattleCellStatus::Blue);
}
//-----------------------------------------------------------------------------
int TargetRangeAttack::findX(int mod) noexcept
{
	const int y = selectPos.y;
	int numX = 0;
	for (int x = 0; x < 3; x++)
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
	for (int y = 0; y < 2; y++)
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