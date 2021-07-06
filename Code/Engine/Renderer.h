#pragma once

#include "Platform.h"

//=============================================================================
// Renderer
//=============================================================================
class OGLRenderer;

class Renderer final : NonCopyable, NonMovable
{
	friend class Engine;
public:
	~Renderer();

private:
	Renderer(Log& log, Window& window) noexcept;
	bool init() noexcept;
	void beginFrame() noexcept;
	void endFrame() noexcept;
	void resize() noexcept;

	Log& m_log;
	Window& m_window;

	OGLRenderer* m_render = nullptr;
};

