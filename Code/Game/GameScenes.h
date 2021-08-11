#pragma once

class IGameCamera;

enum SceneId
{
	SceneId_Intro,
	SceneId_Title,
	SceneId_NewGame,
	SceneId_LoadGame,
	SceneId_Ending,
};

class GameScenes
{
public:
	typedef std::function<void()> FuncNextScene;

	static void SwitchScene(SceneId id);
	static void SwitchToField(int startCol, int startRow);
	static void PerformSceneChange();

	static void BeginFade(int frames, FuncNextScene p);

	static void HideMenu();

	static void Update();
	static void Draw();
};