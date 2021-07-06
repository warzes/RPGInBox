#pragma once

#include "Core.h"

//=============================================================================
// Renderer
//=============================================================================

class OGLRenderer final : NonCopyable, NonMovable
{
public:
	OGLRenderer(Log& log) noexcept;
	~OGLRenderer();

	void ClearMainFrame() noexcept;

	Log& m_log;
};