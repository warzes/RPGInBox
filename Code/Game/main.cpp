#include "stdafx.h"
#include "GameLogic.h"
#include <Engine/DebugNew.h>
#if SE_COMPILER_MSVC
#	pragma comment(lib, "3rdparty.lib")
#	pragma comment(lib, "Engine.lib")
#	if SE_PLATFORM_WINDOWS
#		pragma comment(lib, "winmm.lib")
#	endif
#endif // SE_COMPILER_MSVC

смотреть план и выполнять

пойти в 2д - ведь у меня для того есть код
	тогда за основу плана взять док по рогалику на бейске и вот этот док
	https://bfnightly.bracketproductions.com/rustbook/chapter_2.html
	https://github.com/andrewgasson/roguelike-7drl-2021
	
	
	отказаться от плана с итерациями - а попытаться написать более полный диздок - иначе я никогда не сделаю игру, но при этом буду тратить кучу времени

//-----------------------------------------------------------------------------
void GameMain() noexcept
{
	EngineConfig engineConfig;
	Engine engine;
	if (engine.Init(engineConfig))
	{
		GameLogic game(engine);
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