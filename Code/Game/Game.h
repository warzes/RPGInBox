#pragma once

#include "ResourceManager.h"
#include "FreeCamera.h"
#include "PlayerTurnCamera.h"
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
	FreeCamera m_camera;
	IGameCamera *m_currentCamera = nullptr;
	PlayerTurnCamera m_cameraTurn;
	World m_world;

	bool m_turnCamera = true;

	Model model;
	Texture2D textureModel;

	Model model2;
	Model model3;
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