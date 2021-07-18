#pragma once

#include "IGameCamera.h"

class Sky final : NonCopyable, NonMovable
{
public:
	~Sky();

	bool Init();

	void Update(float deltaTime);
	void Draw(IGameCamera *camera);

private:
	Model m_mountain;
	Texture m_mountTex;
	Model m_sky;
	Texture m_skyTex;
	Shader m_shaderS;
	Shader m_shaderM;
	int m_frameLoc = 0;
	float m_frame = 0.0f;
};