#include "stdafx.h"
#include "Game.h"

#include <Engine/DebugNew.h>
#if SE_COMPILER_MSVC
#	pragma comment(lib, "3rdparty.lib")
#	pragma comment(lib, "Engine.lib")
#	if SE_PLATFORM_WINDOWS
#		pragma comment(lib, "winmm.lib")
#	endif
#endif // SE_COMPILER_MSVC

//https://github.com/andrewgasson/roguelike-7drl-2021
//http://rlgclub.ru/wiki/Давайте_сделаем_рогалик_-_Ричард_Д._Кларк
//https://bfnightly.bracketproductions.com/rustbook/chapter_2.html
//
//https://www.grey-elf.com
//https://www.dragonsfoot.org/fe/
//https://www.youtube.com/channel/UCkN2bSUkP_rSW7ksrJXsWMQ/videos
//https://www.basicfantasy.org/downloads.html
//-----------------------------------------------------------------------------
void GameMain() noexcept
{
	EngineConfig engineConfig;
	engineConfig.window.width = 1900;
	engineConfig.window.height = 1060;
	engineConfig.window.vsync = true;

	Engine engine;
	if (engine.Init(engineConfig))
	{
		Game game(engine);
		if (game.Init())
		{
			while (!engine.IsEnd() && !game.IsEnd())
			{
				engine.Update();
				game.Update(engine.GetDeltaTime());
				//engine.BeginFrame({ 0, 60, 80, 0 });
				game.Frame();
				//engine.EndFrame();
			}
		}		
	}
}
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	GameMain();
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