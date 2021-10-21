#include "stdafx.h"
#include "UIBattlePlayerMenu.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void UIBattlePlayerMenu::Draw() const noexcept
{
	for (size_t i = 0; i < m_elements.size(); i++)
	{
		DrawRectangleLinesEx({ (float)m_elements[i].pos.x, (float)m_elements[i].pos.y, (float)m_elements[i].size.width, (float)m_elements[i].size.height}, 1.0f, WHITE);

		Color color = WHITE;
		if (!m_elements[i].enable)
			color = GRAY;
		if (m_actionElement == (int)i)
			color = GREEN;

		DrawText(m_elements[i].text.c_str(), m_elements[i].pos.x + 10, m_elements[i].pos.y + 5, 30, color);
	}
	if (m_actionElement < (int)m_elements.size())
	{
		DrawText(">>>            <<<", m_leftUpPos.x-40, m_leftUpPos.y + 5 + m_actionElement * m_commonSize.height, 30, GREEN);
	}
}
//-----------------------------------------------------------------------------
void UIBattlePlayerMenu::Update() noexcept
{
	m_select = -1;
	// выбор меню мышкой
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		auto pos = GetMousePosition();
		pos.x -= (float)m_leftUpPos.x;
		pos.y -= (float)m_leftUpPos.y;

		if (pos.x >= 0.0f && pos.x <= (float)m_commonSize.width && pos.y >= 0.0f)
		{
			int iy = (int)(pos.y / (float)m_commonSize.height);
			if (iy < (int)m_elements.size())
			{
				if (iy == m_actionElement) // данный элемент уже выбран, значит игрок выбрал это меню
					m_select = m_actionElement;
				m_actionElement = iy;
			}				
		}
	}
	if (Input::IsPressed(GameKey::Up))
	{
		if (m_actionElement == 0)
			m_actionElement = (int)m_elements.size() - 1;
		else
			--m_actionElement;
	}
	if (Input::IsPressed(GameKey::Down))
	{
		if (m_actionElement >= (int)m_elements.size() - 1)
			m_actionElement = 0;
		else
			++m_actionElement;
	}
	// выбор меню клавишами
	if (Input::IsPressed(GameKey::Ok))
	{
		m_select = m_actionElement;
		m_actionElement = 0; // сброс выбора пункта меню
	}
}
//-----------------------------------------------------------------------------
void UIBattlePlayerMenu::ResetActionElement() noexcept
{
	m_actionElement = 0;
}
//-----------------------------------------------------------------------------