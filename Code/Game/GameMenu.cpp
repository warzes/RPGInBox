#include "stdafx.h"
#include "GameMenu.h"
#include "Sound.h"

void GameMenu::PlayError()
{
	SoundManager::PlayEffect(SEffect_Error);
}

void GameMenu::PlayCursor()
{
	SoundManager::PlayEffect(SEffect_Cursor);
}

void GameMenu::PlayConfirm()
{
	SoundManager::PlayEffect(SEffect_Confirm);
}