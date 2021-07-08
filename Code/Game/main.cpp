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