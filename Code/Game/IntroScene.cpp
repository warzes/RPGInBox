#include "stdafx.h"
#include "IntroScene.h"
#include "GameScenes.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void IntroScene::Init() noexcept
{
	// TODO: здесь можно сделать такой эффект - чтобы оно тут рисовало интро картинку, а потом уже грузило ресурсы (тогда не будет пустого окна при загрузке)
}
//-----------------------------------------------------------------------------
void IntroScene::Update(float /*deltaTime*/) noexcept
{
	GameScenes::SwitchScene(SceneId_MainMenu);
}
//-----------------------------------------------------------------------------
void IntroScene::Draw(IGameCamera* /*camera*/) noexcept
{
}
//-----------------------------------------------------------------------------
IPlayfield* IntroScene::AsPlayfield() noexcept
{
	return nullptr;
}
//-----------------------------------------------------------------------------
IGameModule* IntroScene::Make() noexcept
{
	// TODO: вместо метода можно ведь шаблоном сделать
	IntroScene* mod = new IntroScene();
	mod->Init();
	return mod;
}
//-----------------------------------------------------------------------------
