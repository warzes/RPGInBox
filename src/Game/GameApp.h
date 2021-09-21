#pragma once

#include "GameStateManager.h"
#include "ResourceManager.h"
#include "DataManager.h"

class GameApp final : NonCopyable, NonMovable
{
public:
	GameApp() noexcept;
	~GameApp();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept { return m_gameState.IsEnd(); }

private:
	void createFrame() noexcept;
	void beginFrame() noexcept;
	void endFrame() noexcept;

	DataManager m_data;
	ResourceManager m_resourceMgr;
	GameStateManager m_gameState;

#if MAIN_FRAME_TO_RENDER_TEXTURE
	RenderTexture2D m_target = {};
	Rectangle m_sourceRec = {};
	Rectangle m_destRec = {};
	Vector2 m_origin = { 0.0f, 0.0f };
#endif
};