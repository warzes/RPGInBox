#include "stdafx.h"
#include "IntroScene.h"
#include "GameScenes.h"
#include <Engine/DebugNew.h>

void IntroScene::Init() noexcept
{
	//GameScenes::SwitchScene(SceneId_StartGameMenu);
}

void IntroScene::Update() noexcept
{
}

void IntroScene::Draw() noexcept
{
	DrawText("HELL", 5, 5, 20, WHITE);
}

IPlayfield* IntroScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* IntroScene::Make() noexcept
{
	IntroScene* mod = new IntroScene();
	mod->Init();
	return mod;
}