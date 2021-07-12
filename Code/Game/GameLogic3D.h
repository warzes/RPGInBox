#pragma once

#include "PlayerGameCamera3D.h"
#include "GameData.h"
#include "World.h"

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

	GameData m_data;
	PlayerGameCamera3D m_camera;
	World m_world;

	Image img;
	Texture tx;
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
};