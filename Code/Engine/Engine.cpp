#include "stdafx.h"
#include "Engine.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
Engine::Engine() noexcept
	: window(log)
	, renderer(log, window)
{

}
//-----------------------------------------------------------------------------
Engine::~Engine()
{
	//CloseWindow();
	log.Print("Engine end.");
}
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineConfig& config) noexcept
{
#if SE_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	if (!log.open(config.LogName))
		return false;

	log.Print("Engine Init...");

	InitWindow(config.window.width, config.window.height, "Game");

	//if (!window.init(config.window))
	//	return false;

	//if (!renderer.init())
	//	return false;

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Update() noexcept
{
	//window.update();
	//if (window.IsResize())
	//{
	//	renderer.resize();
	//}

	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
}
//-----------------------------------------------------------------------------
void Engine::BeginFrame() noexcept
{
	//renderer.beginFrame();
}
//-----------------------------------------------------------------------------
void Engine::EndFrame() noexcept
{
	//renderer.endFrame();
}
//-----------------------------------------------------------------------------
bool Engine::IsEnd() const noexcept
{
	//return window.IsClose();
	return WindowShouldClose();
}
//-----------------------------------------------------------------------------