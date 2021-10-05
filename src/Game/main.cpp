#include "stdafx.h"
#include "GameApp.h"
#include "Log.h"
#include "DebugNew.h"

//https://gamefaqs.gamespot.com/pc/564785-pool-of-radiance/faqs/73869
//http://shrines.rpgclassics.com/nes/ff1/whitemagic.shtml
//http://shrines.rpgclassics.com/pc/mm2/characters.shtml
//http://shrines.rpgclassics.com/pc/
//https://gamefaqs.gamespot.com/pc/564789-pools-of-darkness/faqs/8566
//https://media.wizards.com/2018/dnd/downloads/DnD_BasicRules_2018.pdf

//-----------------------------------------------------------------------------
#if SE_COMPILER_MSVC
#	pragma comment(lib, "3rdparty.lib")
#	if SE_PLATFORM_WINDOWS
#		pragma comment(lib, "winmm.lib")
#	endif
#endif // SE_COMPILER_MSVC
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) auto NvOptimusEnablement = 0x00000001UL;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
//-----------------------------------------------------------------------------
inline void Main() noexcept
{
#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF;   // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	{
		bool error = false;

		//---------------------------------------------------------------------
		// Core Init
		//---------------------------------------------------------------------
		Log log("Log.txt");
		log.Print("Engine Init...");

		//---------------------------------------------------------------------
		// Raylib Init
		//---------------------------------------------------------------------
		InitWindow(1024, 768, "Game");
		if (!IsWindowReady())
		{
			log.Error("InitWindow failed!");
			error = true;
		}
		
		if (!error)
		{
			//SetTargetFPS(60);
			InitAudioDevice();
			if (!IsAudioDeviceReady())
			{
				log.Error("Audio device failed!");
				error = true;
			}
		}		

		//---------------------------------------------------------------------
		// Game MainLoop
		//---------------------------------------------------------------------
		if (!error)
		{
			GameApp game;
			if (game.Init())
			{
				while (!WindowShouldClose() && !game.IsEnd())
				{
					game.Update(GetFrameTime());
					game.Frame();
				}
			}
		}

		//---------------------------------------------------------------------
		// Close
		CloseAudioDevice();
		CloseWindowRaylib();
		log.Print("Engine end.");
	}

#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	_CrtDumpMemoryLeaks(); // GLFW каким-то отключает вывод меморилика, поэтому приходится принудительно вызывать
#endif
}
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Main();
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
//	Main();
//	return 0;
//}
#endif // SE_PLATFORM_WINDOWS
//-----------------------------------------------------------------------------