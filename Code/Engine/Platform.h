#pragma once

#include "Core.h"

//=============================================================================
// Input
//=============================================================================

class Input final : NonCopyable, NonMovable
{
	friend class Engine;
public:
private:
	Input() = default;
};

//=============================================================================
// Window
//=============================================================================
struct WindowConfig final
{
	int width = 1024;
	int height = 768;
	bool fullscreen = false;
	bool resizable = true;
	bool vsync = false;
};

class Window final : NonCopyable, NonMovable
{
	friend class Engine;
public:
	~Window();

	bool IsClose() const noexcept { return m_isClose; }
	bool IsActive() const noexcept { return m_isActive; }
	bool IsResize() noexcept { if (m_isResize == true) { m_isResize = false; return true; } return false; }

private:
	Window(Log& log) noexcept;
	bool init(const WindowConfig& config) noexcept;
	void setupFramebuffer(int width, int height) noexcept;

	void update() noexcept;

	Log& m_log;
	struct pimpl;
	pimpl* m_impl = nullptr;

	int m_displayWidth = 0;
	int m_displayHeight = 0;
	int m_width = 0;
	int m_height = 0;
	int m_positionX = 0;
	int m_positionY = 0;
	bool m_fullscreen = false;

	bool m_isClose = true;
	bool m_isActive = true;
	bool m_isResize = true;
};