#pragma once

class IGameCamera;

enum SceneId
{
    SceneId_Intro,
    SceneId_MainMenu,
    SceneId_NewGame,
    SceneId_LoadGame,
    SceneId_Ending,
};

class GameScenes
{
public:
    typedef std::function<void()> FadeEndProc;

	static void SwitchScene(SceneId id) noexcept;
	static void PerformSceneChange() noexcept;

    static void EndFade() noexcept;
    static bool AtFadeEnd() noexcept;

    static void Update(float deltaTime) noexcept;
    static void Draw(IGameCamera* camera) noexcept;
};