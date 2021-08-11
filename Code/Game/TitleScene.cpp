#include "stdafx.h"
#include "TitleScene.h"
#include "GameScenes.h"
#include "Sound.h"
#include "Input.h"
#include "GameMenu.h"
#include <Engine/DebugNew.h>

constexpr int TitleItemX = 390;
constexpr int TitleItemY = 500;
constexpr int OffsetMenu = 50;

TitleScene::TitleScene() noexcept
{
	m_cursor = ResourceManager::GetGlobalTexture("images/cursor.png");

	SoundManager::PlayTrack(Sound_Prelude);
}

void TitleScene::Update() noexcept
{
	if (Input::IsKeyPressing(KEY_DOWN))
	{
		m_selIndex = (m_selIndex + 1) % 3;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		m_selIndex = (m_selIndex - 1 + 3) % 3;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(ConfirmKey))
	{
		GameMenu::PlayConfirm();

		if (m_selIndex == 0)
			GameScenes::SwitchScene(SceneId_NewGame);
		else if (m_selIndex == 1)
			GameScenes::SwitchScene(SceneId_LoadGame);
		else
			; // TODO: exit game
	}
}

void TitleScene::Draw() noexcept
{
	DrawText("NEW GAME",  TitleItemX, TitleItemY, 50, WHITE);
	DrawText("LOAD GAME", TitleItemX, TitleItemY + OffsetMenu, 50, WHITE);
	DrawText("QUIT GAME", TitleItemX, TitleItemY + OffsetMenu * 2, 50, WHITE);

	float y = TitleItemY + 10 + m_selIndex * OffsetMenu;
	DrawTextureEx(*m_cursor, { TitleItemX - 45, y }, 0.0f, 2.0f, WHITE);
}

IPlayfield* TitleScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* TitleScene::Make() noexcept
{
	TitleScene* mod = new TitleScene();
	return mod;
}