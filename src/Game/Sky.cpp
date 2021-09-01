#include "stdafx.h"
#include "Sky.h"
#include "DebugNew.h"
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
bool Sky::Init() noexcept
{
	m_mountain = LoadModel("../resources/mountain.glb");
	m_mountTex = LoadTexture("../resources/mountain.png");
	m_mountain.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_mountTex;

	m_sky = LoadModel("../resources/sky.glb");
	m_skyTex = LoadTexture("../resources/cloud.png");
	SetTextureFilter(m_skyTex, TEXTURE_FILTER_BILINEAR);
	m_sky.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_skyTex;

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
void Sky::Update(float deltaTime) noexcept
{
	m_frame += m_speedAnimationSky * deltaTime;
	SetShaderValue(m_shaderS, m_frameLoc, &m_frame, SHADER_UNIFORM_FLOAT);
}
//-----------------------------------------------------------------------------
void Sky::Draw(const Camera3D& refCamera, const Vector3& cameraPos) noexcept
{
	BeginMode3D(refCamera);
	DrawModel(m_mountain, cameraPos, 1.0f, WHITE);
	DrawModel(m_sky, cameraPos, 1.0f, WHITE);
	EndMode3D();
}
//-----------------------------------------------------------------------------