#include "stdafx.h"
#include "MainMenuScene.h"
#include "Sound.h"
#include "Text.h"
#include "Input.h"
#include "GameScenes.h"
#include "GlobalMenu.h"
#include <Engine/DebugNew.h>

const int TitleItemX = 96;
const int TitleItemY = 96;


MainMenuScene::MainMenuScene() noexcept
{
	SoundManager::PlayTrack(Sound_Prelude, 0, true);
}

void MainMenuScene::Update(float deltaTime) noexcept
{
	if (Input::IsKeyPressing(KEY_DOWN))
	{
		selIndex = (selIndex + 1) % 2;
		MainMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		selIndex = (selIndex - 1 + 2) % 2;
		MainMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_Z))
	{
		MainMenu::PlayConfirm();

		if (selIndex == 0)
			GameScenes::SwitchScene(SceneId_NewGame);
		else if (selIndex == 1)
			GameScenes::SwitchScene(SceneId_LoadGame);
	}
}

void MainMenuScene::Draw(IGameCamera* camera) noexcept
{
	Text::DrawString("NEW GAME", TitleItemX, TitleItemY);
	Text::DrawString("LOAD GAME", TitleItemX, TitleItemY + 16);

	int y = TitleItemY + selIndex * 16;

	Text::DrawCursor(TitleItemX - 16, y);
}

IPlayfield* MainMenuScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* MainMenuScene::Make() noexcept
{
	return new MainMenuScene();
}