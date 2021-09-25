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
		//if (!m_elements[i].enable)
		//	color = GRAY;
		if (m_actionElement == i)
			color = GREEN;

		DrawText(m_elements[i].text.c_str(), m_elements[i].pos.x + 10, m_elements[i].pos.y + 5, 30, color);
	}
	if (m_actionElement >= 0 && m_actionElement < m_elements.size())
	{
		DrawText(">>>            <<<", m_leftUpPos.x-40, m_leftUpPos.y + 5 + m_actionElement * m_commonSize.height, 30, GREEN);
	}
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::Run() noexcept
{
	m_select = -1;
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		auto pos = GetMousePosition();
		pos.x -= m_leftUpPos.x;
		pos.y -= m_leftUpPos.y;

		if (pos.x >= 0 && pos.y >= 0)
		{
			int iy = pos.y / m_commonSize.height;

			if (pos.x <= m_commonSize.width && iy < m_elements.size())
			{
				if (iy == m_actionElement) // ������ ������� ��� ������, ������ ����� ������ ��� ����
					m_select = m_actionElement;
				m_actionElement = iy;
			}				
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::Reset(const Point2& leftUpPos) noexcept
{
	m_elements.clear();
	m_leftUpPos = leftUpPos;
	m_commonSize = { 140, 40 };
}
//-----------------------------------------------------------------------------
void GameBattlePlayerMenu::AddElement(const std::string& text) noexcept
{
	const int y = m_elements.size() * 40;
	m_elements.push_back({ text, {m_leftUpPos.x, m_leftUpPos.y+y}, m_commonSize });
}
//-----------------------------------------------------------------------------
//void GameBattlePlayerMenu::SetEnable(bool enable, int idElement) noexcept
//{
//	if (idElement >= 0 && idElement < m_elements.size())
//		m_elements[idElement].enable = enable;
//}
//-----------------------------------------------------------------------------