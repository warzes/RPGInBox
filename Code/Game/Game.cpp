#include "stdafx.h"
#include "Game.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
class Game::GameFrame final
{
public:
	GameFrame()
	{
#if MAIN_FRAME_TO_RENDER_TEXTURE
		const int screenWidth = GetScreenWidth();
		const int screenHeight = GetScreenHeight();

#if OLD_SCHOOL_RENDER
		const int virtualScreenWidth = 320;
		const int virtualScreenHeight = 240;
#else
		const int virtualScreenWidth = screenWidth;
		const int virtualScreenHeight = screenHeight;
#endif

		const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

		m_target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);
		// The target's height is flipped (in the source Rectangle), due to OpenGL reasons
		m_sourceRec = { 0.0f, 0.0f, (float)m_target.texture.width, -(float)m_target.texture.height };
		m_destRec = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2) };
#endif
	}

	~GameFrame()
	{
#if MAIN_FRAME_TO_RENDER_TEXTURE
		UnloadRenderTexture(m_target);
#endif
	}

	void BeginFrame()
	{
		// Set render target
#if MAIN_FRAME_TO_RENDER_TEXTURE
		BeginTextureMode(m_target);
		ClearBackground({ 0, 60, 80, 0 });
#else
		BeginDrawing();
		ClearBackground({ 0, 60, 80, 0 });
#endif
	}
	void EndFrame()
	{
		// end render target
#if MAIN_FRAME_TO_RENDER_TEXTURE
		EndTextureMode();
		BeginDrawing();
		ClearBackground({ 0, 60, 80, 0 });
		DrawTexturePro(m_target.texture, m_sourceRec, m_destRec, m_origin, 0.0f, WHITE);
#endif
		DrawFPS(0, 0);
		EndDrawing();
	}

private:
#if MAIN_FRAME_TO_RENDER_TEXTURE
	RenderTexture2D m_target = {};
	Rectangle m_sourceRec = {};
	Rectangle m_destRec = {};
	Vector2 m_origin = { 0.0f, 0.0f };
#endif
};
//-----------------------------------------------------------------------------
Game::Game(Engine& engine) noexcept
	: m_engine(engine)
	, m_world(m_resourceMgr)
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
	delete m_gameFrame;
}
//-----------------------------------------------------------------------------
bool Game::Init() noexcept
{
	m_cameraTurn.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;
	if (m_turnCamera) m_camera.HideCursor = false;

	if (m_turnCamera) m_currentCamera = &m_cameraTurn;
	else m_currentCamera = &m_camera;

	if (!m_world.Init())
		return false;

	auto playerPos = m_world.playerParty.GetPosition();
	m_cameraTurn.SetCameraPosition({ (float)playerPos.x, 0.0f, (float)playerPos.y }, 0.0f);

	m_gameFrame = new Game::GameFrame();

	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void Game::Update(float deltaTime) noexcept
{
	m_world.Update(deltaTime);
	// TODO: возможно логику камеру также перенести в m_world?
	if (m_world.environment.GetStatus() == GameStatus::Exploring)
	{
		m_currentCamera->Update(m_world);
		if (!m_cameraTurn.IsProcessMoving())
		{
			auto const& camPos = m_currentCamera->GetCameraPosition();
			m_world.Move({ (int)camPos.x, (int)camPos.z });
		}
	}
}
//-----------------------------------------------------------------------------
void Game::Frame() noexcept
{
	m_gameFrame->BeginFrame();
	{
		m_world.Draw(m_currentCamera);
	}	
	m_gameFrame->EndFrame();
}
//-----------------------------------------------------------------------------