#include "stdafx.h"
#include "Engine.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
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
//void Engine::BeginFrame(Color&& color) noexcept
//{
//	BeginDrawing();
//	ClearBackground(std::move(color));
//}
////-----------------------------------------------------------------------------
//void Engine::EndFrame() noexcept
//{
//	DrawFPS(0, 0);
//	EndDrawing();
//}
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