#pragma once

#include "Core.h"

struct WindowConfig final
{
	int width = 1280;
	int height = 768;
	bool fullscreen = false;
	bool resizable = true;
	bool vsync = true;
};

struct EngineConfig final
{
	const char* LogName = "Log.txt";

	WindowConfig window;
};

class Engine final : NonCopyable, NonMovable
{
public:
	~Engine();

	bool Init(const EngineConfig& config) noexcept;

	void Update() noexcept;
	//void BeginFrame(Color&& color) noexcept;
	//void EndFrame() noexcept;

	bool IsEnd() const noexcept;

	float GetDeltaTime() const noexcept;

	Log log;
};