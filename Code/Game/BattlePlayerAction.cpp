#include "stdafx.h"
#include "BattlePlayerAction.h"
#include "BattleCore.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void BattlePlayerAction::Reset() noexcept
{
	m_selectPlayerCommand = -1;
	m_playerCommandState = playerCommandState::SelectCommand;
	m_selectOneEnemyTagetInMelee = false;
	m_selectOneEnemyTagetInRange = false;
}
//-----------------------------------------------------------------------------
bool BattlePlayerAction::PlayerAction() noexcept
{
	// ������� ����� ������� �� ������
	if (m_playerCommandState == playerCommandState::SelectCommand)
	{
		selectPlayerCommand(); // ����� �������
	}
	//... ���

	return false;
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::Draw() noexcept
{
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::DrawCommandList() noexcept
{
	for (int i = 0; i < Countof(playerCommandRect); i++)
		DrawRectangleLinesEx(playerCommandRect[i], 1.0f, WHITE);

	if (m_selectPlayerCommand >= 0)
		DrawText(">>>            <<<", 770, 420 + m_selectPlayerCommand * 40, 30, GREEN);

	DrawText("Attack", 820, 420, 30, WHITE);
	DrawText("Skill", 820, 460, 30, WHITE);
	DrawText("Magic", 820, 500, 30, WHITE);
	DrawText("Defence", 820, 540, 30, WHITE);
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::selectPlayerCommand() noexcept
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const auto mousePos = GetMousePosition();
		int select = 0;
		for (; select < Countof(playerCommandRect); select++)
		{
			const auto& rect = playerCommandRect[select];
			if (mousePos.x > rect.x && mousePos.x < rect.x + rect.width &&
				mousePos.y > rect.y && mousePos.y < rect.y + rect.height)
				break;
		}
		if (select < Countof(playerCommandRect))
		{
			m_selectPlayerCommand = select;
			m_selectOneEnemyTagetInMelee = false;
			m_selectOneEnemyTagetInRange = false;
			if (m_selectPlayerCommand == 0)
				m_selectOneEnemyTagetInMelee = true;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		m_selectOneEnemyTagetInMelee = false;
		m_selectPlayerCommand = -1; // ������ ��������� ������� �� ������ ������ ����
	}
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::selectAttackTargetEnemy() noexcept
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const auto mousePos = GetMousePosition();
		int cell = 0;
		for (; cell < Countof(battleCell); cell++)
		{
			const auto& rect = battleCell[cell];
			if (mousePos.x > rect.x && mousePos.x < rect.x + rect.width &&
				mousePos.y > rect.y && mousePos.y < rect.y + rect.height)
				break;
		}
		if (cell < Countof(battleCell))
		{
			//if (m_currentMember >= 0 && m_currentMember < 3 && cell >= 3 && cell < 6)
			//{
			//	const unsigned x = cell % ColumnWidthBattleCells;
			//	const unsigned y = cell / ColumnWidthBattleCells;

			//	printf("%d - %d:%d\n", cell, x + 1, y + 1);
			//}
		}
	}
}