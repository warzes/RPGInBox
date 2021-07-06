#pragma once

#include "Platform.h"

//=============================================================================
// Renderer
//=============================================================================

class Renderer final : NonCopyable, NonMovable
{
	friend class Engine;
public:
	Renderer(Log& log, Window& window) noexcept;
	~Renderer();

private:
	bool init() noexcept;
	void beginFrame() noexcept;
	void endFrame() noexcept;
	void resize() noexcept;

	Log& m_log;
	Window& m_window;
};

