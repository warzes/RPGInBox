#pragma once

#include "Log.h"

struct EngineConfig final
{
	const char* LogName = "Log.txt";

	struct
	{
		int width = 1280;
		int height = 768;
		bool fullscreen = false;
		bool resizable = true;
		bool vsync = true;
	} window;
};

class Engine final : NonCopyable, NonMovable
{
public:
	~Engine();

	bool Init(const EngineConfig& config) noexcept;

	bool IsEnd() const noexcept;

	float GetDeltaTime() const noexcept;

private:
	Log m_log;
};