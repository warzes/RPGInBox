#include "stdafx.h"
#include "GameScenes.h"
#include "IGameModule.h"
#include "IntroScene.h"
#include "TitleScene.h"
#include "NewGameScene.h"
#include "SaveLoadScene.h"
#include "Overworld.h"
#include <Engine/DebugNew.h>

enum SceneAction
{
	Scene_None,
	Scene_SwitchScene,
	Scene_SwitchToField,
	Scene_PushLevel,
	Scene_PopLevel,
	Scene_PopAllLevels,
};

struct SceneChange
{
	SceneAction Action;
	int Col;
	int Row;
	int Level;
};

struct SceneFrame
{
	int8_t  Level;
	uint8_t Col;
	uint8_t Row;
	uint8_t InRoom;
};

typedef IGameModule* (*SceneMaker)();

SceneChange pendingScene;

SceneFrame stack[100];
int stackLength = 0;
int curLevelId = 0;
bool inShop = false;

IGameModule* curScene = nullptr;
IGameModule* curOverlay = nullptr;

bool fade = false;
int fadeTimer = 0;
int fadeFrames = 0;
GameScenes::FuncNextScene funcNextScene = nullptr;

SceneMaker sceneMakers[] =
{
	IntroScene::Make,
	TitleScene::Make,
	NewGameScene::Make,
	SaveLoadScene::MakeLoad,
	//EndingScene::Make,
};
//-----------------------------------------------------------------------------
static IGameModule* MakeScene(int id)
{
	if (id < 0 || id >= _countof(sceneMakers))
		return IntroScene::Make();
	SceneMaker maker = sceneMakers[id];
	return maker();
}
//-----------------------------------------------------------------------------
static void SwitchScene()
{
	delete curOverlay;
	curOverlay = nullptr;

	delete curScene;
	curScene = nullptr;

	IGameModule* scene = MakeScene(pendingScene.Level);

	curScene = scene;
	curLevelId = -1;
	stackLength = 0;
}
//-----------------------------------------------------------------------------
static void SwitchToField()
{
	delete curOverlay;
	curOverlay = nullptr;

	delete curScene;
	curScene = nullptr;

	Overworld* field = new Overworld();
	field->Init(pendingScene.Col, pendingScene.Row);

	curScene = field;
	curLevelId = -1;
	stackLength = 0;
}
//-----------------------------------------------------------------------------
void GameScenes::SwitchScene(SceneId id)
{
	pendingScene.Action = Scene_SwitchScene;
	pendingScene.Level = id;
}
//-----------------------------------------------------------------------------
void GameScenes::SwitchToField(int startCol, int startRow)
{
	pendingScene.Action = Scene_SwitchToField;
	pendingScene.Col = startCol;
	pendingScene.Row = startRow;
}
//-----------------------------------------------------------------------------
void GameScenes::PerformSceneChange()
{
	if (pendingScene.Action != Scene_None)
	{
		// scene changes override fades
		fade = false;
	}

	switch (pendingScene.Action)
	{
	case Scene_SwitchScene:
		::SwitchScene();
		break;

	case Scene_SwitchToField:
	   ::SwitchToField();
	    break;

	case Scene_PushLevel:
	//    ::PushLevel();
		;
	    break;

	case Scene_PopLevel:
	//    ::PopLevel();
		;
	    break;

	case Scene_PopAllLevels:
	//    ::PopAllLevels();
		;
	    break;
	}

	pendingScene.Action = Scene_None;
}
//-----------------------------------------------------------------------------
void GameScenes::BeginFade(int frames, FuncNextScene p)
{
	fade = true;
	fadeTimer = 0;
	fadeFrames = frames;
	funcNextScene = p;
}
//-----------------------------------------------------------------------------
void GameScenes::HideMenu()
{
	delete curOverlay;
	curOverlay = nullptr;

	if (curScene != nullptr)
	{
		IPlayfield* playfield = curScene->AsPlayfield();

		if (playfield != nullptr && inShop)
			playfield->HandleShopClosed();
	}
}
//-----------------------------------------------------------------------------
void GameScenes::Update()
{
	if (curOverlay != nullptr)
		curOverlay->Update();
	else
		curScene->Update();
	PerformSceneChange();
}
//-----------------------------------------------------------------------------
void GameScenes::Draw()
{
	if (curOverlay != nullptr)
		curOverlay->Draw();
	else
		curScene->Draw();
}
//-----------------------------------------------------------------------------