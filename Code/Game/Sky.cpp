#include "stdafx.h"
#include "Sky.h"
#include <Engine/DebugNew.h>

//-----------------------------------------------------------------------------
Sky::~Sky()
{
	UnloadShader(m_shaderS);
	UnloadShader(m_shaderM);
	UnloadModel(m_mountain);
	UnloadTexture(m_mountTex);
	UnloadModel(m_sky);
	UnloadTexture(m_skyTex);
}
//-----------------------------------------------------------------------------
bool Sky::Init()
{
	m_mountain = LoadModel("../resources/mountain.glb");
	m_mountTex = LoadTexture("../resources/mountain.png");
	m_mountain.materials[0].maps[MAP_DIFFUSE].texture = m_mountTex;

	m_sky = LoadModel("../resources/sky.glb");
	m_skyTex = LoadTexture("../resources/cloud.png");
#if OLD_SCHOOL_RENDER
	SetTextureFilter(m_skyTex, TEXTURE_FILTER_POINT);
#else
	SetTextureFilter(m_skyTex, FILTER_BILINEAR);
#endif	
	m_sky.materials[0].maps[MAP_DIFFUSE].texture = m_skyTex;

	// sky shader (animated)
	m_shaderS = LoadShader("../resources/shader.vs", "../resources/shader.fs");
	m_shaderS.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(m_shaderS, "matModel");
	m_shaderS.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(m_shaderS, "viewPos");

	// mountain shader (discards)
	m_shaderM = LoadShader("../resources/shader.vs", "../resources/discard.fs");
	m_shaderM.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(m_shaderM, "matModel");
	m_shaderM.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(m_shaderM, "viewPos");

	// set the models shader
	m_sky.materials[0].shader = m_shaderS;
	m_mountain.materials[0].shader = m_shaderM;

	// frame counter
	m_frameLoc = GetShaderLocation(m_shaderS, "frame");

	return true;
}
//-----------------------------------------------------------------------------
void Sky::Update(float deltaTime)
{
	m_frame += 0.04f * deltaTime;
	SetShaderValue(m_shaderS, m_frameLoc, &m_frame, SHADER_UNIFORM_FLOAT);
}
//-----------------------------------------------------------------------------
void Sky::Draw(IGameCamera* camera)
{
	BeginMode3D(camera->GetCamera());
	DrawModel(m_mountain, camera->GetCameraPosition(), 1, WHITE);
	DrawModel(m_sky, camera->GetCameraPosition(), 1, WHITE);
	EndMode3D();
}
//-----------------------------------------------------------------------------