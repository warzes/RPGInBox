#pragma once

class IGameCamera;

enum SceneId
{
	SceneId_Intro,
	SceneId_StartGameMenu,
	SceneId_NewGame,
	SceneId_LoadGame,
	SceneId_Ending,
};

class GameScenes
{
public:
	typedef std::function<void()> FuncNextScene;

	static void SwitchScene(SceneId id);

	static void PerformSceneChange();

	static void Update();
	static void Draw();
};