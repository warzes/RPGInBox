#include "stdafx.h"
#include "GameLogic3D.h"
#include <Engine/DebugNew.h>
#if SE_COMPILER_MSVC
#	pragma comment(lib, "3rdparty.lib")
#	pragma comment(lib, "Engine.lib")
#	if SE_PLATFORM_WINDOWS
#		pragma comment(lib, "winmm.lib")
#	endif
#endif // SE_COMPILER_MSVC

https://github.com/andrewgasson/roguelike-7drl-2021
http://rlgclub.ru/wiki/Давайте_сделаем_рогалик_-_Ричард_Д._Кларк
https://bfnightly.bracketproductions.com/rustbook/chapter_2.html

https://www.grey-elf.com
https://www.dragonsfoot.org/fe/
https://www.youtube.com/channel/UCkN2bSUkP_rSW7ksrJXsWMQ/videos
https://www.basicfantasy.org/downloads.html



//-----------------------------------------------------------------------------
void GameMain() noexcept
{
	EngineConfig engineConfig;
	Engine engine;
	if (engine.Init(engineConfig))
	{
		GameLogic3D game(engine);
		if (game.Init())
		{
			while (!engine.IsEnd() && !game.IsEnd())
			{
				engine.Update();
				game.Update(engine.GetDeltaTime());
				engine.BeginFrame({ 0, 60, 80, 0 });
				game.Frame();
				engine.EndFrame();
			}
		}		
	}
}

static bool appRunning;

void QuitApplication(void)
{
	appRunning = false;
}

void GameMainRoguelike() noexcept
{
	// Setup raylib
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(640, 480, "7DRL 2021");
	SetTargetFPS(30);

	// Setup terminal
	InitTerminal(80, 26);
	LoadTerminalFont("..\\Asset\\VGA9x16.png", 1);
	SetWindowSize(GetTerminalScreenWidth(), GetTerminalScreenHeight());

	// Stop immediate seizures
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();

	// Initialize modules (order matters only in regards to memory locality, 
	// otherwise, because modules are relational, they do not depend on 
	// each other for initialization)
	//InitInput();
	//InitTerrain(GetTerminalWidth(), GetTerminalHeight(), 1);
	//InitItemPrefabs();
	//InitActors(ACTOR_CAPACITY);
	//InitContainers(CONTAINER_CAPACITY);
	//InitCreatures(CREATURE_CAPACITY);
	//InitDoors(DOOR_CAPACITY);
	//InitInventories(INVENTORY_CAPACITY);
	//InitSprites(SPRITE_CAPACITY);

	//// Start engine loop
	//SetView(&VIEW_MAIN_MENU);
	appRunning = true;

	while (!WindowShouldClose() && appRunning) {
		//UpdateInput();
		//ControlView();
		ClearTerminal();

		//if (ShouldProcessGameActors()) {
		//	// TODO: Any view that requests a pause turn process should be a block (sort of the opposite of world render)
		//	// If it is the player's turn, skip,
		//	// else if there are no actors to process, process
		//	// the round
		//	// else process up to the maximum actors
		//}

		//if (ShouldRenderGameWorld()) {
		//	RenderTerrain(0);
		//	RenderSprites();
		//}

		//RenderViews();
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTerminal();
		EndDrawing();
	}

	// Let the platform cleanup, yet ensure at least the window or app 
	// instance is closed
	CloseWindowRaylib();
}
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	//GameMain();
	GameMainRoguelike();
	return 0;
}
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
//int WINAPI wWinMain(
//	[[maybe_unused]] _In_ HINSTANCE hInstance,
//	[[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
//	[[maybe_unused]] _In_ LPWSTR lpCmdLine,
//	[[maybe_unused]] _In_ int nCmdShow)
//{
//	GameMain();
//	return 0;
//}
#endif // SE_PLATFORM_WINDOWS
//-----------------------------------------------------------------------------