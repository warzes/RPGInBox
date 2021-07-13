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
#if OLD_SCHOOL_RENDER
	UnloadRenderTexture(target);
#endif
	UnloadTexture(tx);
}
//-----------------------------------------------------------------------------
bool GameLogic3D::Init() noexcept
{
	m_camera.Setup(45.0f, Vector3{ 0.0f, 0.0f, 0.0f });
#if !TURN_STEP
	m_camera.MoveSpeed.z = 10;
	m_camera.MoveSpeed.x = 5;
#endif
	if (!m_world.Init())
		return false;

	auto playerPos = m_world.playerParty.GetPosition();

	m_camera.SetCameraPosition({ (float)playerPos.x, 0.0f, (float)playerPos.y }, 0.0f);

	img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
	tx = LoadTextureFromImage(img);
	UnloadImage(img);
#if OLD_SCHOOL_RENDER
	SetTextureFilter(tx, TEXTURE_FILTER_POINT);
#else
	SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
#endif
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	cubeTx = LoadTexture("../resources/test.png");
	cube = LoadModelFromMesh(GenMeshCube(.6, .6, .6));
	cube.materials[0].maps[MAP_DIFFUSE].texture = cubeTx;

	mountain = LoadModel("../resources/mountain.glb");
	mountTx = LoadTexture("../resources/mountain.png");
	mountain.materials[0].maps[MAP_DIFFUSE].texture = mountTx;

	sky = LoadModel("../resources/sky.glb");
	skyTx = LoadTexture("../resources/cloud.png");
#if OLD_SCHOOL_RENDER
	SetTextureFilter(skyTx, TEXTURE_FILTER_POINT);
#else
	SetTextureFilter(skyTx, FILTER_BILINEAR);
#endif	
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


#if OLD_SCHOOL_RENDER
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();

	const int virtualScreenWidth = 160;
	const int virtualScreenHeight = 120;

	const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

	target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight); // This is where we'll draw all our objects.

	 // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
	sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
	destRec = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2) };
#endif

	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void GameLogic3D::Update(float deltaTime) noexcept
{
	frame += 0.04 * deltaTime;
	SetShaderValue(shaderS, frameLoc, &frame, SHADER_UNIFORM_FLOAT);

	m_camera.Update(m_world);
}
//-----------------------------------------------------------------------------
void GameLogic3D::Frame() noexcept
{
	// Set render target
#if OLD_SCHOOL_RENDER
	BeginTextureMode(target);
	ClearBackground({ 0, 60, 80, 0 });
#else
	BeginDrawing();
	ClearBackground({ 0, 60, 80, 0 });
#endif
	{
		// World map
		{
			BeginMode3D(m_camera.GetCamera());
			m_camera.ExtractFrustum();
			for (int x = 0; x < MapSize; x++)
			{
				for (int y = 0; y < MapSize; y++)
				{
					if (m_world.openworld.tiles[x][y].isTree)
					{
						const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
						const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };
						if (m_camera.GetFrustum().AABBoxIn(min, max))
						{
							DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
							DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
						}
					}
				}
			}

			EndMode3D();
		}

		// Grid
		{
			int slices = 20;
			float spacing = 1.0f;

			BeginMode3D(m_camera.GetCamera());

			rlCheckRenderBatchLimit((slices + 2) * 4);

			rlBegin(RL_LINES);
			for (int i = -slices; i <= slices; i++)
			{
				rlColor3f(0.75f, 0.75f, 0.75f);
				rlColor3f(0.75f, 0.75f, 0.75f);
				rlColor3f(0.75f, 0.75f, 0.75f);
				rlColor3f(0.75f, 0.75f, 0.75f);

				rlVertex3f((float)i * spacing - 0.5f, 0.0f, (float)-slices * spacing - 0.5f);
				rlVertex3f((float)i * spacing - 0.5f, 0.0f, (float)slices * spacing - 0.5f);

				rlVertex3f((float)-slices * spacing - 0.5f, 0.0f, (float)i * spacing - 0.5f);
				rlVertex3f((float)slices * spacing - 0.5f, 0.0f, (float)i * spacing - 0.5f);
			}
			rlEnd();
			EndMode3D();
		}

		// Sky
		{
			BeginMode3D(m_camera.GetCamera());
			DrawModel(mountain, m_camera.GetCameraPosition(), 1, WHITE);
			DrawModel(sky, m_camera.GetCameraPosition(), 1, WHITE);
			EndMode3D();
		}
	}

	// end render target
#if OLD_SCHOOL_RENDER
	EndTextureMode();
	BeginDrawing();
	ClearBackground({ 0, 60, 80, 0 });
	DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
#endif

	// Text
	{
		auto pos = m_world.playerParty.GetPosition();
		std::string text = "Player pos {x=" + std::to_string(pos.x) + " y=" + std::to_string(pos.y) + "}";
		DrawText(text.c_str(), 10, 30, 20, WHITE);
	}

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------