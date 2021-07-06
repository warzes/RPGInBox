#pragma once

#include "Core.h"

struct WindowConfig final
{
	int width = 1024;
	int height = 768;
	bool fullscreen = false;
	bool resizable = true;
	bool vsync = false;
};

struct EngineConfig final
{
	const char* LogName = "Log.txt";

	WindowConfig window;
};

class Engine final : NonCopyable, NonMovable
{
public:
	Engine() noexcept;
	~Engine();

	bool Init(const EngineConfig& config) noexcept;

	void Update() noexcept;
	void BeginFrame() noexcept;
	void EndFrame() noexcept;

	bool IsEnd() const noexcept;

	Log log;
};