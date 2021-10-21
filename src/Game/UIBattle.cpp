#include "stdafx.h"
#include "UIBattle.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr Point2 LeftTopCoordCells = { 262, 44 };
//-----------------------------------------------------------------------------
UIBattle::UIBattle(ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool UIBattle::Init() noexcept
{
	// �������� �������
	if (!m_background.Create(&m_resourceMgr))
		return false;

	m_battleBackGround = m_resourceMgr.GetTexture("../data/temp/textures/character/plains-ground.png");

	return true;
}
//-----------------------------------------------------------------------------
void UIBattle::Draw() const noexcept
{
	// ��������� ������� ���� ���� ���
	m_background.Draw();

	// ��������� ����
	DrawTextureTiled(*m_battleBackGround, { 0.0f, 0.0f, 64.0f, 64.0f }, { (float)LeftTopCoordCells.x, (float)LeftTopCoordCells.y, 500.0f, 680.0f }, { 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);

	// ��������� ����� ���������� ���� ���
	DrawRectangle((int)LeftTopCoordCells.x, 383, 500, 3, WHITE);

	drawPanels();
}
//-----------------------------------------------------------------------------
void UIBattle::drawPanels() const noexcept
{
	// ��������� ������� �������

	// ����� ������� ������
	{
		DrawRectangle(29, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("Enemy", 135, 100, 38, RED);
		DrawText("Enemy", 135, 270, 38, RED);
	}

	// ����� ������ ������
	{
		DrawRectangle(29, 390, 230, 335, { 0, 0, 0, 180 });
		DrawText("Player", 125, 450, 38, GREEN);
		DrawText("Player", 125, 620, 38, GREEN);
	}

	// ������ ������� ������
	{
		DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("INFO WINDOW", 775, 50, 30, WHITE);
	}

	// ������ ������ ������
	{
		DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });
	}
}
//-----------------------------------------------------------------------------