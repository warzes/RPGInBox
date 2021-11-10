#include "stdafx.h"
#include "GameApp.h"
#include "Log.h"
#include "DebugNew.h"

//https://assetstore.unity.com/packages/3d/environments/fantasy/fantasy-environment-level-set-18598

/*
магия
	концетрация (если есть навык)
	. находится в меню магии:
		концетрация
		уровень 1
		уровень 2
		уровень 3
		(уровни только если доступны)
		
	при использовании маг пропускает ход и получает одно очко концетрации. Можно собрать до интеллект+уровень кол-ва.
	если в следующем ходу маг применяет заклинание, то все накопленные очки тратятся, а заклинание получает бонусы концетрации (в зависимости от кол-ва очков). Любое действие мага обнуляет очки концетрации (но заклинания активируют эффекты концетрации).
	Полученный урон вызывает проверку концетрации, при неудаче также теряются все очки (при удаче в зависимости от талантов может быть потеря нескольких очков, одного очка или вообще без потерь)	
	Концетрация позволяет сосредоточиться и жахнуть по максимуму
	
	Стратегическое мышление (талант) - любые пассивные действия пропускающие ход (защита, пропуск хода, применение предмета) дают очко концетрации
		усиление таланта - активные действия не обнуляют очки, а делают проверку, при удаче очки сохраняются (дальнейшие усиления могут довести до гаранта)
		
	У жрецов примерно тоже самое - но называется молитвой
	
	
Эссенции
	Возможно вместо очков магии или вместе с очками можно использовать эссенции. Каждое заклинание относится к одной или нескольким стихиям, эссенции нужных стихий добавляют бонусы (или позволяют использовать это заклинание если откажусь от очков магии, то есть если нет ессенции воды, то маг не сможет использовать магию огня - очень неплохо например в пустыне, где вода будет имбовать, но ресурсов под нее не найти... Так что наверное откажусь от очков магии)
	Эссенции нужны чтобы не заморачиваться всяким пометом летучей мыши. Просто собираются разные компоненты, и из них алхимией выделяется нужная стихия (ну там огня, воды и т.д.)
	
Мир
	летающие острова - для дна сделать туман (хотя бы даже мешем как небо). Это позволит сделать генерацию мира
	
	
	а еще мир из той игры про торговца или жнеца на рпгмекере




https://www.gamebanshee.com/eyeofthebeholderiii/spells/clevel1.php
http://goldbox.pbworks.com/w/page/8901459/Pool%20of%20Radiance
https://www.gamebanshee.com/poolofradiance/
https://www.gamebanshee.com/poolofradiance/attributes.php
https://www.gamebanshee.com/poolofradiance/races.php
https://www.gamebanshee.com/poolofradiance/classes.php
https://www.gamebanshee.com/poolofradiance/spells/magelevelone.php
https://www.gamebanshee.com/poolofradiance/bestiary/t-z.php

https://www.dandwiki.com/wiki/Regional_Maps
https://www.dndbeyond.com/homebrew
*/

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
			SetTargetFPS(60);

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
					const float dt = GetFrameTime();
					game.Update(dt);
					game.Frame();
				}
			}
		}

		//---------------------------------------------------------------------
		// Close
		CloseAudioDevice();
		CloseRaylib();
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