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
	bool init() noexcept;
	void update() noexcept;

	Log& m_log;

	bool m_isClose = true;
	bool m_isActive = true;
	bool m_isResize = true;
};