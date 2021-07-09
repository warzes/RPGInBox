#pragma once

#include "PlayerGameCamera3D.h"
#include "GameData.h"

#define SKY_ENABLE 1

class GameLogic3D final : NonCopyable, NonMovable
{
public:
	GameLogic3D(Engine& engine) noexcept;
	~GameLogic3D();

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
	PlayerGameCamera3D m_camera;

	GameData m_data;

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