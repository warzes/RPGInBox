#pragma once

#include "IGameCamera.h"

class Sky final : NonCopyable, NonMovable
{
public:
	~Sky();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Draw(IGameCamera *camera) noexcept;

private:
	Model m_mountain;
	Texture m_mountTex;
	Model m_sky;
	Texture m_skyTex;
	Shader m_shaderS;
	Shader m_shaderM;
	int m_frameLoc = 0;
	float m_frame = 0.0f;
	const float m_speedAnimationSky = 0.04f;
};