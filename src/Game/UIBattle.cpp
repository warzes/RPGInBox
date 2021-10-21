#include "stdafx.h"
#include "UIBattle.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 PlayerMenuLeftTopPos = { 810, 415 };
//-----------------------------------------------------------------------------
UIBattle::UIBattle(ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool UIBattle::Init() noexcept
{
	// загрузка текстур
	if (!m_background.Init(&m_resourceMgr))
		return false;

	m_battleBackGround = m_resourceMgr.GetTexture("../data/temp/textures/character/plains-ground.png");

	// создание меню
	m_playerMenu.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu);
	m_playerMenu_attack.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu_Attack);

	return true;
}
//-----------------------------------------------------------------------------
void UIBattle::Update() noexcept
{
	if (m_currentPlayerMenu) m_currentPlayerMenu->Update();
}
//-----------------------------------------------------------------------------
void UIBattle::Draw() const noexcept
{
	// отрисовка заднего фона окна боя
	m_background.Draw();

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0.0f, 0.0f, 64.0f, 64.0f }, { (float)LeftTopCoordCells.x, (float)LeftTopCoordCells.y, 500.0f, 680.0f }, { 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);

	// отрисовка линии разделения поля боя
	DrawRectangle((int)LeftTopCoordCells.x, 383, 500, 3, WHITE);

	drawPanels();

	if (m_currentPlayerMenu) m_currentPlayerMenu->Draw();
}
//-----------------------------------------------------------------------------
int UIBattle::SelectMenu() const noexcept
{
	if (!m_currentPlayerMenu) return -1;
	return m_currentPlayerMenu->IsSelect();
}
//-----------------------------------------------------------------------------
void UIBattle::ResetPlayerMenu() noexcept
{
	m_currentPlayerMenu = nullptr;
}
//-----------------------------------------------------------------------------
void UIBattle::ActivePlayerMainMenu() noexcept
{
	m_currentPlayerMenu = &m_playerMenu;
	m_currentPlayerMenu->ResetActionElement(); // TODO: в будущем подумать над системой сохранения выбора меню, например в виде массива команд
}
//-----------------------------------------------------------------------------
void UIBattle::ActivePlayerMenuAttack() noexcept
{
	m_currentPlayerMenu = &m_playerMenu_attack;
	m_currentPlayerMenu->ResetActionElement(); // TODO: в будущем подумать над системой сохранения выбора меню, например в виде массива команд
}
//-----------------------------------------------------------------------------
void UIBattle::drawPanels() const noexcept
{
	// отрисовка боковых панелей

	// левая верхняя панель
	{
		DrawRectangle(29, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("Enemy", 135, 100, 38, RED);
		DrawText("Enemy", 135, 270, 38, RED);
	}

	// левая нижняя панель
	{
		DrawRectangle(29, 390, 230, 335, { 0, 0, 0, 180 });
		DrawText("Player", 125, 450, 38, GREEN);
		DrawText("Player", 125, 620, 38, GREEN);
	}

	// правая верхняя панель
	{
		DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("INFO WINDOW", 775, 50, 30, WHITE);
	}

	// правая нижняя панель
	{
		DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });
	}
}
//-----------------------------------------------------------------------------