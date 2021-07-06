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
	log.Print("Engine end.");
}
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineConfig& config) noexcept
{
	m_config = config;

#if SE_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	if (!log.open(config.LogName))
		return false;

	log.Print("Engine Init...");

	if (!window.init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Update() noexcept
{
	window.update();
	if (window.IsResize())
	{
		renderer.resize();
	}
}
//-----------------------------------------------------------------------------
void Engine::BeginFrame() noexcept
{
	renderer.beginFrame();
}
//-----------------------------------------------------------------------------
void Engine::EndFrame() noexcept
{
	renderer.endFrame();
}
//-----------------------------------------------------------------------------
bool Engine::IsEnd() const noexcept
{
	return window.IsClose();
}
//-----------------------------------------------------------------------------