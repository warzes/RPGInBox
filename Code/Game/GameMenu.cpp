#include "stdafx.h"
#include "GameMenu.h"
#include "GameScenes.h"
#include "Sound.h"
#include <Engine/DebugNew.h>

GameMenu* GameMenu::instance = nullptr;

GameMenu::GameMenu() noexcept
{
	instance = this;
}

GameMenu::~GameMenu()
{
	instance = nullptr;
	popAll();
}

void GameMenu::InitHost(Menu* firstMenu) noexcept
{
	init();
	// MainMenu takes ownership of this menu
	m_activeMenu = firstMenu;
	m_activeMenu->prevMenu = nullptr;
}

void GameMenu::Update() noexcept
{
	MenuAction action = Menu_None;
	Menu* nextMenu = nullptr;

	action = m_activeMenu->Update(nextMenu);

	switch (action)
	{
	case Menu_Pop:
		pop();
		if (m_activeMenu == nullptr)
			GameScenes::HideMenu();
		break;

	case Menu_Push:
		push(nextMenu);
		break;

	case Menu_SwitchTop:
		popAll();
		push(nextMenu);
		break;
	}
}

void GameMenu::Draw() noexcept
{
	m_activeMenu->Draw(MenuDraw_Active);
}

IPlayfield* GameMenu::AsPlayfield() noexcept
{
	return nullptr;
}

void GameMenu::DrawClass(int classId, float x, float y) noexcept
{
	float sy = classId * 72;
	DrawTexturePro(*instance->m_playerBmp, { 0, sy, 48, 72 }, { x, y, 48, 72 }, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void GameMenu::DrawOrb(int index, bool lit, float x, float y) noexcept
{
	float sx = 0;

	if (!lit)
		sx = 5 * 16;
	else
		sx = 16 + index * 16;

	DrawTexturePro(*instance->m_menuBmp, { sx, 0, 16, 16 }, { x, y, 16, 16 }, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void GameMenu::PlayError() noexcept
{
	SoundManager::PlayEffect(SEffect_Error);
}

void GameMenu::PlayCursor() noexcept
{
	SoundManager::PlayEffect(SEffect_Cursor);
}

void GameMenu::PlayConfirm() noexcept
{
	SoundManager::PlayEffect(SEffect_Confirm);
}

void GameMenu::init() noexcept
{
	if (!LoadList("itemTarget.dat", m_itemTarget, _countof(m_itemTarget)))
		return;

	if (!LoadList("magicTarget.dat", m_magicTarget, _countof(m_magicTarget)))
		return;

	if (!LoadResource("menuText.tab", &m_menuText))
		return;

	if (!LoadList("shopTypes.dat", m_shopTypes, _countof(m_shopTypes)))
		return;

	if (!LoadResource("shopStock.tab", &m_shopStock))
		return;

	if (!LoadResource("shopText.tab", &m_shopText))
		return;

	m_playerBmp = ResourceManager::GetGlobalTexture("images/playerSprites.png");
	m_menuBmp = ResourceManager::GetGlobalTexture("images/menu.png");	
}

void GameMenu::push(Menu* nextMenu) noexcept
{
	if (nextMenu != nullptr)
	{
		nextMenu->prevMenu = m_activeMenu;
		m_activeMenu = nextMenu;
	}
}

void GameMenu::pop() noexcept
{
	Menu* menu = m_activeMenu;
	m_activeMenu = m_activeMenu->prevMenu;
	delete menu;
}

void GameMenu::popAll() noexcept
{
	while (m_activeMenu != nullptr)
	{
		pop();
	}
}