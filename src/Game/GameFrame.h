#pragma once

class GameFrame final
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