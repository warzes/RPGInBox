#include "stdafx.h"
#include "IntroScene.h"
#include "GameScenes.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void IntroScene::Init() noexcept
{
	// TODO: ����� ����� ������� ����� ������ - ����� ��� ��� �������� ����� ��������, � ����� ��� ������� ������� (����� �� ����� ������� ���� ��� ��������)
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
	// TODO: ������ ������ ����� ���� �������� �������
	IntroScene* mod = new IntroScene();
	mod->Init();
	return mod;
}
//-----------------------------------------------------------------------------
