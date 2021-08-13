#include "stdafx.h"
#include "OldGame.h"
#include "Game.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
#if SE_COMPILER_MSVC
#	pragma comment(lib, "3rdparty.lib")
#	pragma comment(lib, "Engine.lib")
#	if SE_PLATFORM_WINDOWS
#		pragma comment(lib, "winmm.lib")
#	endif
#endif // SE_COMPILER_MSVC
//-----------------------------------------------------------------------------
void GameMain() noexcept
{
#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF;   // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	{
		EngineConfig engineConfig;
		engineConfig.window.width = 1024;
		engineConfig.window.height = 768;
		engineConfig.window.vsync = true;

		Engine engine;
		if (engine.Init(engineConfig))
		{
			//OldGame game(engine);
			Game game(engine);
			if (game.Init())
			{
				while (!engine.IsEnd()  && !game.IsEnd())
				{
					game.Update(engine.GetDeltaTime());
					game.Frame();
				}
			}
		}
	}

#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	_CrtDumpMemoryLeaks();
#endif
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