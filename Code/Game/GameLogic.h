#pragma once

#include "PlayerGameCamera.h"

#define SKY_ENABLE 1

class GameLogic final : NonCopyable, NonMovable
{
public:
	GameLogic(Engine& engine) noexcept;
	~GameLogic();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept
	{
		return m_isEnd;
	}

private:
	Engine& m_engine;
	bool m_isEnd = true;

	Image img;
	Texture tx;
	PlayerGameCamera m_camera;

#if SKY_ENABLE
	Texture cubeTx;
	Model cube;
	Model mountain;
	Texture mountTx;
	Model sky;
	Texture skyTx;
	Shader shaderS;
	Shader shaderM;
	int frameLoc;
	float frame = 0;
#endif
};