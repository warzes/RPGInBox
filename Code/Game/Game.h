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

	bool IsEnd() const noexcept { return m_isEnd; }

private:
	Engine& m_engine;
	bool m_isEnd = true;

	ResourceManager m_resourceMgr;
	FreeCamera m_camera;
	PlayerTurnCamera m_cameraTurn;
	IGameCamera *m_currentCamera = nullptr;
	World m_world;

	bool m_turnCamera = true;

#if MAIN_FRAME_TO_RENDER_TEXTURE
	RenderTexture2D m_target = {};
	Rectangle m_sourceRec = {};
	Rectangle m_destRec = {};
	Vector2 m_origin = { 0.0f, 0.0f };
#endif
};