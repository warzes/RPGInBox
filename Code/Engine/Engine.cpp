#include "stdafx.h"
#include "Engine.h"
#include "FPCamera.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
Engine::Engine() noexcept
{
}
//-----------------------------------------------------------------------------
Engine::~Engine()
{
	CloseWindowRaylib();
	log.Print("Engine end.");
}
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineConfig& config) noexcept
{
#if SE_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF;   // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	if (!log.open(config.LogName))
		return false;

	log.Print("Engine Init...");

	InitWindow(config.window.width, config.window.height, "Game");
	if (config.window.vsync)
		SetTargetFPS(60);

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Update() noexcept
{

}
//-----------------------------------------------------------------------------
void Engine::BeginFrame(Color&& color) noexcept
{
	BeginDrawing();
	ClearBackground(std::move(color));
}
//-----------------------------------------------------------------------------
void Engine::EndFrame() noexcept
{
	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
bool Engine::IsEnd() const noexcept
{
	return WindowShouldClose();
}
//-----------------------------------------------------------------------------
float Engine::GetDeltaTime() const noexcept
{
	return GetFrameTime();
}
//-----------------------------------------------------------------------------