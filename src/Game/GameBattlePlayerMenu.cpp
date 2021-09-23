#include "stdafx.h"
#include "GameBattlePlayerMenu.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::Draw() noexcept
{
	for (size_t i = 0; i < m_elements.size(); i++)
	{
		DrawRectangleLinesEx({ (float)m_elements[i].pos.x, (float)m_elements[i].pos.y, (float)m_elements[i].size.width, (float)m_elements[i].size.height}, 1.0f, WHITE);

		Color color = WHITE;
		if (!m_elements[i].enable)
			color = GRAY;
		if (m_select == i)
			color = GREEN;

		DrawText(m_elements[i].text.c_str(), m_elements[i].pos.x + 10, m_elements[i].pos.y + 5, 30, color);
	}
	if (m_select >= 0 && m_select < m_elements.size())
	{
		DrawText(">>>            <<<", 770, 420 + m_select * 40, 30, GREEN);
	}
}
//-----------------------------------------------------------------------------
int GameBattlePlayerMenu::Run() noexcept
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		auto pos = GetMousePosition();
		pos.x -= 810;
		pos.y -= 415;

		if (pos.x >= 0 && pos.y >= 0)
		{
			int iy = pos.y / 40;

			if (pos.x <= 140 && iy < m_elements.size())
				return iy;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::Reset() noexcept
{
	m_elements.clear();
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::AddElement(const std::string& text) noexcept
{
	const int y = m_elements.size() * 40;
	m_elements.push_back({ text, true, {810, 415+y}, {140, 40} });
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::SetSelect(int sel) noexcept
{
	m_select = sel;
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::SetEnable(bool enable, int idElement) noexcept
{
	if (idElement >= 0 && idElement < m_elements.size())
		m_elements[idElement].enable = enable;
}
//-----------------------------------------------------------------------------