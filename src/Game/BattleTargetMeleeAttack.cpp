#include "stdafx.h"
#include "BattleTargetMeleeAttack.h"
#include "BattleCell.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
TargetMeleeAttack::TargetMeleeAttack() noexcept
{
	target.resize(3);
	for (int x = 0; x < 3; x++)
	{
		target[x] = nullptr;
	}
}
//-----------------------------------------------------------------------------
bool TargetMeleeAttack::IsZero() const noexcept
{
	for (int x = 0; x < 3; x++)
	{
		if (target[x] != nullptr) return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
void TargetMeleeAttack::SetPos(int x) noexcept
{
	if (target[x] != nullptr)
	{
		selectPos = x;
	}
	else
	{
		// TODO: улучшить - чтобы выбирался оптимальный
		for (int x = 0; x < 3; x++)
		{
			if (target[x] != nullptr)
			{
				selectPos = x;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void TargetMeleeAttack::Update() noexcept
{
	for (int x = 0; x < 3; x++)
	{
		if (target[x] != nullptr)
		{
			target[x]->SetStatus(BattleCellStatus::Green);
		}
	}

	if (Input::IsPressed(GameKey::Left))
	{
		selectPos = findX(-1);
	}
	if (Input::IsPressed(GameKey::Right))
	{
		selectPos = findX(1);
	}
	if (selectPos >= 0 && selectPos < 3 )
		target[selectPos]->SetStatus(BattleCellStatus::Blue);
}
//-----------------------------------------------------------------------------
int TargetMeleeAttack::findX(int mod) noexcept
{
	int numX = 0;
	for (int x = 0; x < 3; x++)
	{
		if (target[x] != nullptr) numX++;
	}
	if (numX == 0) return -1;          // никого нет в этом ряду
	if (numX == 1) return selectPos; // некуда двигать, он один

	int x = selectPos;

	if (mod == -1)
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numX) break; // если ушло в бесконечный цикл
			if (x == 0) x = 2;
			else x--;
			if (target[x]) break;
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
			if (target[x]) break;
		}
	}

	return x;
}
//-----------------------------------------------------------------------------