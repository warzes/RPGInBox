#include "stdafx.h"
#include "IntroScene.h"
#include "GameScenes.h"
#include <Engine/DebugNew.h>

void IntroScene::Update() noexcept
{
	GameScenes::SwitchScene(SceneId_Title);
}

void IntroScene::Draw() noexcept
{
}

IPlayfield* IntroScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* IntroScene::Make() noexcept
{
	IntroScene* mod = new IntroScene();
	return mod;
}