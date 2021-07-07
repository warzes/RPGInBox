#pragma once

#include "PlayerGameCamera.h"

class GameLogic final : NonCopyable, NonMovable
{
public:
	GameLogic(Engine& engine) noexcept;
	~GameLogic();

	bool Init() noexcept;

	bool Update() noexcept;
	void Frame() noexcept;

private:
	Engine& m_engine;

	Image img;
	Texture tx;
	PlayerGameCamera m_camera;
};