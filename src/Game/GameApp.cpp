#include "stdafx.h"
#include "GameApp.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
GameApp::GameApp() noexcept
	: m_gameState(m_resourceMgr, m_data)
{
}
//-----------------------------------------------------------------------------
GameApp::~GameApp()
{
#if MAIN_FRAME_TO_RENDER_TEXTURE
	UnloadRenderTexture(m_target);
#endif
}
//-----------------------------------------------------------------------------
bool GameApp::Init() noexcept
{
	createFrame();

	m_data.player.StartDefaultParty();

	if (!m_gameState.Init())
		return false;

	m_gameState.SetState(GameState::BeginBattle);
	return true;
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime) noexcept
{
	m_gameState.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void GameApp::Frame() noexcept
{
	beginFrame();
	m_gameState.Frame();
	endFrame();
}
//-----------------------------------------------------------------------------
void GameApp::createFrame() noexcept
{
#if MAIN_FRAME_TO_RENDER_TEXTURE
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();

#	if OLD_SCHOOL_RENDER
	const int virtualScreenWidth = 320;
	const int virtualScreenHeight = 240;
#	else
	const int virtualScreenWidth = screenWidth;
	const int virtualScreenHeight = screenHeight;
#	endif // OLD_SCHOOL_RENDER

	const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

	m_target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);
	// The target's height is flipped (in the source Rectangle), due to OpenGL reasons
	m_sourceRec = { 0.0f, 0.0f, (float)m_target.texture.width, -(float)m_target.texture.height };
	m_destRec = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2) };
#endif // MAIN_FRAME_TO_RENDER_TEXTURE
}
//-----------------------------------------------------------------------------
void GameApp::beginFrame() noexcept
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
//-----------------------------------------------------------------------------
void GameApp::endFrame() noexcept
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
//-----------------------------------------------------------------------------