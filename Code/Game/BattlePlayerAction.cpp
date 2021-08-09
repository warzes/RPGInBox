#include "stdafx.h"
#include "BattlePlayerAction.h"
#include "BattleCore.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void BattlePlayerAction::Reset() noexcept
{
	m_selectPlayerCommand = -1;
	m_playerCommandState = PlayerCommandState::SelectCommand;
}
//-----------------------------------------------------------------------------
bool BattlePlayerAction::PlayerAction() noexcept
{
	// ожидаем выбор команды от игрока
	if (m_playerCommandState == PlayerCommandState::SelectCommand)
	{
		selectPlayerCommand(); // выбор команды
	}
	else if (m_playerCommandState == PlayerCommandState::SelectMeleeAttackTarget)
	{
		selectMeleeAttackTarget(); // выбор цели ближней атаки
	}

	return false;
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::DrawCommandList() noexcept
{
	for (size_t i = 0; i < Countof(playerCommandRect); i++)
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
		size_t select = 0;
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
			if (m_selectPlayerCommand == 0)
				m_playerCommandState = PlayerCommandState::SelectMeleeAttackTarget;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		m_selectPlayerCommand = -1; // отмена выбранной команды по правой кнопке мыши
	}
}
//-----------------------------------------------------------------------------
void BattlePlayerAction::selectMeleeAttackTarget() noexcept
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
			if (cell >= 3 && cell < 6)
			{
				const unsigned x = cell % ColumnWidthBattleCells;
				const unsigned y = cell / ColumnWidthBattleCells;

				printf("%d - %d:%d\n", cell, x + 1, y + 1);
			}
		}
	}
}