﻿#pragma once

#include "Log.h"

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

	bool IsEnd() const noexcept;

	float GetDeltaTime() const noexcept;

private:
	Log m_log;
};