#include "stdafx.h"
#include "GameScenes.h"
#include "IGameModule.h"
#include "IntroScene.h"
#include "MainMenuScene.h"
#include "NewGameScene.h"
#include "SaveLoadScene.h"
#include "EndingScene.h"
#include "Overworld.h"
#include "Level.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
enum SceneAction
{
	Scene_None,
	Scene_SwitchScene,
	Scene_SwitchToField,
	Scene_PushLevel,
	Scene_PopLevel,
	Scene_PopAllLevels,
};
//-----------------------------------------------------------------------------
struct SceneChange
{
	SceneAction Action;
	int Col;
	int Row;
	int Level;
};
//-----------------------------------------------------------------------------
struct SceneFrame
{
	int8_t  Level;
	uint8_t Col;
	uint8_t Row;
	uint8_t InRoom;
};
//-----------------------------------------------------------------------------
typedef IGameModule* (*SceneMaker)();
SceneMaker sceneMakers[] =
{
	IntroScene::Make,
	MainMenuScene::Make,
	NewGameScene::Make,
	SaveLoadScene::MakeLoad,
	EndingScene::Make,
};
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
GameScenes::FadeEndProc fadeEndProc;
//-----------------------------------------------------------------------------
void GameScenes::SwitchScene(SceneId id) noexcept
{
	pendingScene.Action = Scene_SwitchScene;
	pendingScene.Level = id;
}
//-----------------------------------------------------------------------------
IGameModule* makeScene(int id) noexcept
{
	if (id < 0 || id >= _countof(sceneMakers))
		return MainMenuScene::Make();
	SceneMaker maker = sceneMakers[id];
	return maker();
}
//-----------------------------------------------------------------------------
void switchScene() noexcept
{
	delete curOverlay;
	curOverlay = nullptr;
	delete curScene;
	curScene = nullptr;
	IGameModule* scene = makeScene(pendingScene.Level);
	curScene = scene;
	curLevelId = -1;
	stackLength = 0;
}
//-----------------------------------------------------------------------------
void switchToField() noexcept
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
void pushLevel() noexcept
{
	int inRoom = 0;
	Point2 pos = { 0 };

	if (curScene != nullptr && stackLength < _countof(stack))
	{
		IPlayfield* playfield = curScene->AsPlayfield();

		if (playfield != nullptr)
		{
			inRoom = playfield->GetInRoom();
			pos = playfield->GetCurrentPos();

			stack[stackLength].Level = curLevelId;
			stack[stackLength].Col = pos.X;
			stack[stackLength].Row = pos.Y;
			stack[stackLength].InRoom = inRoom;
			stackLength++;
		}
	}

	delete curOverlay;
	curOverlay = nullptr;

	delete curScene;
	curScene = nullptr;

	Level* level = new Level();
	level->Init(pendingScene.Level, pendingScene.Col, pendingScene.Row, inRoom);

	curScene = level;
	curLevelId = pendingScene.Level;
}
//-----------------------------------------------------------------------------
void popLevel() noexcept
{
	if (stackLength <= 0)
		return;

	delete curOverlay;
	curOverlay = nullptr;

	delete curScene;
	curScene = nullptr;

	stackLength--;
	SceneFrame& frame = stack[stackLength];

	if (frame.Level == -1)
	{
		Overworld* field = new Overworld();
		field->Init(frame.Col, frame.Row);

		curScene = field;
		curLevelId = -1;
	}
	else
	{
		Level* level = new Level();
		level->Init(
			frame.Level,
			frame.Col,
			frame.Row,
			frame.InRoom);

		curScene = level;
		curLevelId = frame.Level;
	}
}
//-----------------------------------------------------------------------------
void popAllLevels() noexcept
{
	if (stackLength > 1)
		stackLength = 1;

	popLevel();
}
//-----------------------------------------------------------------------------
void GameScenes::PerformSceneChange() noexcept
{
	if (pendingScene.Action != Scene_None)
	{
		// scene changes override fades
		fade = false;
	}

	switch (pendingScene.Action)
	{
	case Scene_SwitchScene:
		::switchScene();
		break;

	case Scene_SwitchToField:
		::switchToField();
		break;

	case Scene_PushLevel:
		::pushLevel();
		break;

	case Scene_PopLevel:
		::popLevel();
		break;

	case Scene_PopAllLevels:
		::popAllLevels();
		break;
	}

	pendingScene.Action = Scene_None;
}
//-----------------------------------------------------------------------------
void GameScenes::EndFade() noexcept
{
	fade = false;
}
//-----------------------------------------------------------------------------
bool GameScenes::AtFadeEnd() noexcept
{
	return fadeTimer >= fadeFrames;
}
//-----------------------------------------------------------------------------
void updateFade()
{
	if (!fade) return;

	if (fadeTimer < fadeFrames)
		fadeTimer++;

	if (GameScenes::AtFadeEnd())
	{
		if (fadeEndProc)
		{
			GameScenes::EndFade();
			fadeEndProc();
			GameScenes::PerformSceneChange();
		}
	}
}
//-----------------------------------------------------------------------------
void GameScenes::Update(float deltaTime) noexcept
{
	if (curOverlay != nullptr)
		curOverlay->Update(deltaTime);
	else
		curScene->Update(deltaTime);

	PerformSceneChange();
	updateFade();
}
//-----------------------------------------------------------------------------
void GameScenes::Draw(IGameCamera* camera) noexcept
{
	// TODO: а может оверлей поверх сцены?
	if (curOverlay != nullptr)
		curOverlay->Draw(camera);
	else
		curScene->Draw(camera);
}
//-----------------------------------------------------------------------------