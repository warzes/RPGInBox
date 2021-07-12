#include "stdafx.h"
#include "GameLogic3D.h"
//-----------------------------------------------------------------------------
GameLogic3D::GameLogic3D(Engine& engine) noexcept
	: m_engine(engine)
{
}
//-----------------------------------------------------------------------------
GameLogic3D::~GameLogic3D()
{
#if SKY_ENABLE
	Texture cubeTx;
	Model cube;
	Model mountain;
	Texture mountTx;
	Model sky;
	Texture skyTx;
	Shader shaderS;
	Shader shaderM;
	int frameLoc;
	float frame = 0;
#endif
	UnloadTexture(tx);
}
//-----------------------------------------------------------------------------
bool GameLogic3D::Init() noexcept
{
	img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
	tx = LoadTextureFromImage(img);
	UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	m_camera.Setup(45.0f, Vector3{ 0.0f, 0.0f, 0.0f });
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;

#if SKY_ENABLE
	// texture and shade a models

	cubeTx = LoadTexture("../resources/test.png");
	cube = LoadModelFromMesh(GenMeshCube(.6, .6, .6));
	cube.materials[0].maps[MAP_DIFFUSE].texture = cubeTx;

	mountain = LoadModel("../resources/mountain.glb");
	mountTx = LoadTexture("../resources/mountain.png");
	mountain.materials[0].maps[MAP_DIFFUSE].texture = mountTx;

	sky = LoadModel("../resources/sky.glb");
	skyTx = LoadTexture("../resources/cloud.png");
	SetTextureFilter(skyTx, FILTER_BILINEAR);
	sky.materials[0].maps[MAP_DIFFUSE].texture = skyTx;

	// sky shader (animated)
	shaderS = LoadShader("../resources/shader.vs", "../resources/shader.fs");
	shaderS.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shaderS, "matModel");
	shaderS.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(shaderS, "viewPos");

	// mountain shader (discards)
	shaderM = LoadShader("../resources/shader.vs", "../resources/discard.fs");
	shaderM.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shaderM, "matModel");
	shaderM.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(shaderM, "viewPos");

	// set the models shader
	sky.materials[0].shader = shaderS;
	mountain.materials[0].shader = shaderM;

	// frame counter
	frameLoc = GetShaderLocation(shaderS, "frame");
#endif

	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void GameLogic3D::Update(float deltaTime) noexcept
{
	m_camera.Update();
#if SKY_ENABLE
	frame += 0.04 * deltaTime;
	SetShaderValue(shaderS, frameLoc, &frame, SHADER_UNIFORM_FLOAT);
#endif
}
//-----------------------------------------------------------------------------
void GameLogic3D::Frame() noexcept
{
#if SKY_ENABLE
	{
		BeginMode3D(m_camera.GetCamera());
		m_camera.ExtractFrustum();

		for (int i = -64; i < 64; i = i + 2)
		{
			{
				Vector3 min = { i - 0.5f, 0, -0.5f };
				Vector3 max = { i + 0.5f, 1, 0.5f };
				if (m_camera.GetFrustum().AABBoxIn(min, max))
				{
					DrawModel(cube, Vector3{ (float)i, .4, 0 }, 1, WHITE);
				}
			}
			{
				Vector3 min = { -0.5f,0,i - 0.5f };
				Vector3 max = { 0.5f,1,i + 0.5f };
				if (m_camera.GetFrustum().AABBoxIn(min, max))
				{
					DrawModel(cube, Vector3{ 0, .4, (float)i }, 1, WHITE);
				}
			}
		}

		DrawModel(mountain, m_camera.GetCameraPosition(), 1, WHITE);
		DrawModel(sky, m_camera.GetCameraPosition(), 1, WHITE);

		EndMode3D();	
	}
#endif
	{
		BeginMode3D(m_camera.GetCamera());
		m_camera.ExtractFrustum();

		// grid of cube trees on a plane to make a "world"
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, PURPLE); // simple world plane
		const float spacing = 4;
		const int count = 5;
		for (float x = -count * spacing; x <= count * spacing; x += spacing)
		{
			for (float z = -count * spacing; z <= count * spacing; z += spacing)
			{
				Vector3 pos = { x, 0.5f, z };

				Vector3 min = { x - 0.5f,0,z - 0.5f };
				Vector3 max = { x + 0.5f,1,z + 0.5f };
				if (m_camera.GetFrustum().AABBoxIn(min, max))
				{
					DrawCubeTexture(tx, Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
					DrawCubeTexture(tx, Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
				}
			}
		}

		DrawGrid(20, 1);         // Draw a grid

		EndMode3D();
	}
}
//-----------------------------------------------------------------------------