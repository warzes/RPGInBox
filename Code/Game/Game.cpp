#include "stdafx.h"
#include "Game.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
Game::Game(Engine& engine) noexcept
	: m_engine(engine)
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
#if MAIN_FRAME_TO_RENDER_TEXTURE
	UnloadRenderTexture(m_target);
#endif
}
//-----------------------------------------------------------------------------
bool Game::Init() noexcept
{
	m_cameraTurn.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.Setup(45.0f, { 0.0f, 0.0f, 0.0f });
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;
	if (m_turnCamera) m_currentCamera = &m_cameraTurn;
	else m_currentCamera = &m_camera;

	if (!m_world.Init(m_resourceMgr))
		return false;

	auto playerPos = m_world.playerParty.GetPosition();
	m_cameraTurn.SetCameraPosition({ (float)playerPos.x, 0.0f, (float)playerPos.y }, 0.0f);

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
	// Set render target
#if MAIN_FRAME_TO_RENDER_TEXTURE
	BeginTextureMode(m_target);
	ClearBackground({ 0, 60, 80, 0 });
#else
	BeginDrawing();
	ClearBackground({ 0, 60, 80, 0 });
#endif

	m_world.Draw(m_currentCamera);
	// Grid
	/*{
		int slices = 30;
		float spacing = 1.0f;
	
		Point2 currentPosGrid = { 0, 0 };
#if TURN_STEP
		currentPosGrid = m_world.playerParty.GetPosition();
#endif
	
		BeginMode3D(m_camera.GetCamera());
	
		rlCheckRenderBatchLimit((slices + 2) * 4);
	
		rlBegin(RL_LINES);
		for (int i = -slices; i <= slices; i++)
		{
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
	
			rlVertex3f(
				(float)currentPosGrid.x+i * spacing - 0.5f, 
				0.0f, 
				(float)currentPosGrid.y + -slices * spacing - 0.5f);
			rlVertex3f(
				(float)currentPosGrid.x + i * spacing - 0.5f, 
				0.0f, 
				(float)currentPosGrid.y + slices * spacing - 0.5f);
	
			rlVertex3f(
				(float)currentPosGrid.x + -slices * spacing - 0.5f, 
				0.0f, 
				(float)currentPosGrid.y + i * spacing - 0.5f);
			rlVertex3f(
				(float)currentPosGrid.x + slices * spacing - 0.5f, 
				0.0f, 
				(float)currentPosGrid.y + i * spacing - 0.5f);
		}
		rlEnd();
		EndMode3D();
	}*/

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
//-----------------------------------------------------------------------------