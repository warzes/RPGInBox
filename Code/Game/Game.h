#pragma once

#include "ResourceManager.h"
#include "PlayerGameCamera3D.h"
#include "World.h"

class Game final : NonCopyable, NonMovable
{
public:
	Game(Engine& engine) noexcept;
	~Game();

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

	ResourceManager m_resourceMgr;
	PlayerGameCamera3D m_camera;
	World m_world;

	Model model;
	Texture2D textureModel;

	Model model2;
	Texture2D textureModel2;

	//Texture2D texGlass;

#if OLD_SCHOOL_RENDER
	RenderTexture2D target = {};
	Rectangle sourceRec = {};
	Rectangle destRec = {};
	Vector2 origin = { 0.0f, 0.0f };
#endif

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