#include "stdafx.h"
#include "BattleTargetMeleeAttack.h"
#include "BattleCell.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
TargetMeleeAttack::TargetMeleeAttack() noexcept
{
	target.resize(MaxNumberCellInBattle);
	for (int x = 0; x < MaxNumberCellInBattle; x++)
	{
		target[x] = nullptr;
	}
}
//-----------------------------------------------------------------------------
bool TargetMeleeAttack::IsZero() const noexcept
{
	for (int x = 0; x < MaxNumberCellInBattle; x++)
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
		// TODO: �������� - ����� ��������� �����������
		for (int x = 0; x < MaxNumberCellInBattle; x++)
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
	for (int x = 0; x < MaxNumberCellInBattle; x++)
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
	if (selectPos >= 0 && selectPos < MaxNumberCellInBattle)
		target[selectPos]->SetStatus(BattleCellStatus::Blue);
}
//-----------------------------------------------------------------------------
int TargetMeleeAttack::findX(int mod) noexcept
{
	int numX = 0;
	for (int x = 0; x < MaxNumberCellInBattle; x++)
	{
		if (target[x] != nullptr) numX++;
	}
	if (numX == 0) return -1;          // ������ ��� � ���� ����
	if (numX == 1) return selectPos; // ������ �������, �� ����

	int x = selectPos;

	if (mod == -1)
	{
		int err = 0;
		while (1)
		{
			err++;
			if (err > numX) break; // ���� ���� � ����������� ����
			if (x == 0) x = MaxNumberCellInBattle-1;
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
			if (err > numX) break; // ���� ���� � ����������� ����
			if (x == MaxNumberCellInBattle - 1) x = 0;
			else x++;
			if (target[x]) break;
		}
	}

	return x;
}
//-----------------------------------------------------------------------------