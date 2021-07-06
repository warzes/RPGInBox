#pragma once

#include "Renderer.h"

struct EngineConfig final
{
	const char* LogName = "Log.txt";
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
	Input input;
	Window window;
	Renderer renderer;
private:
	EngineConfig m_config;
};