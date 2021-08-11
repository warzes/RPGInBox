#include "stdafx.h"
#include "SaveLoadScene.h"
#include "SaveLoadMenu.h"
#include <Engine/DebugNew.h>

void SaveLoadScene::InitSave() noexcept
{
	SaveLoadMenu* menu = new SaveLoadMenu(SaveLoadMenu::Save);
	m_mainMenu.InitHost(menu);
}

void SaveLoadScene::InitLoad() noexcept
{
	SaveLoadMenu* menu = new SaveLoadMenu(SaveLoadMenu::Load);
	m_mainMenu.InitHost(menu);
}

void SaveLoadScene::Update() noexcept
{
	m_mainMenu.Update();
}

void SaveLoadScene::Draw() noexcept
{
	m_mainMenu.Draw();
}

IPlayfield* SaveLoadScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* SaveLoadScene::MakeSave() noexcept
{
	SaveLoadScene* screen = new SaveLoadScene();
	screen->InitSave();
	return screen;
}

IGameModule* SaveLoadScene::MakeLoad() noexcept
{
	SaveLoadScene* screen = new SaveLoadScene();
	screen->InitLoad();
	return screen;
}