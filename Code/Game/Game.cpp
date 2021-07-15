#include "stdafx.h"
#include "Game.h"
#include <Engine/DebugNew.h>
TODO("game state");
//-----------------------------------------------------------------------------
Game::Game(Engine& engine) noexcept
	: m_engine(engine)
{
}
//-----------------------------------------------------------------------------
Game::~Game()
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
	UnloadTexture(textureModel);     // Unload texture
	UnloadModel(model);         // Unload model
	UnloadTexture(tx);
}
//-----------------------------------------------------------------------------
bool Game::Init() noexcept
{
	if (!m_resourceMgr.Init())
		return false;

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

	const int virtualScreenWidth = 320;
	const int virtualScreenHeight = 240;

	const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

	target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight); // This is where we'll draw all our objects.

	 // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
	sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
	destRec = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2) };
#endif


	model = LoadModel("../data/temp/model/tile/floor01.obj"); // Load model
	textureModel = LoadTexture("../data/temp/model/tile/tileset.png"); // Load model texture
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureModel; // Set map diffuse texture
	//BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);
	model2 = LoadModel("../data/temp/model/tempmap.obj"); // Load model
	textureModel2 = LoadTexture("../data/temp/model/temptile.png"); // Load model texture
	model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureModel2; // Set map diffuse texture

	model3 = LoadModel("../data/temp/model/tree.obj"); // Load model
	model3.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureModel2; // Set map diffuse texture


	//texGlass = LoadTexture("../data/temp/textures/map/outdoor/plains-ground.png"); // Load model texture

	m_isEnd = false;
	return true;
}
//-----------------------------------------------------------------------------
void Game::Update(float deltaTime) noexcept
{
	frame += 0.04 * deltaTime;
	SetShaderValue(shaderS, frameLoc, &frame, SHADER_UNIFORM_FLOAT);

	m_camera.Update(m_world);
}
//-----------------------------------------------------------------------------
void Game::Frame() noexcept
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
		//не нужно перебирать всю карту, у меня есть направление камеры, вот начиная от игрока и в глубь идти (можно даже прерывать если точно за текущим тайлом не будет видно остальных - типа стены в данже)
		//	перебор всего оставить для свободной камеры

		// World map
		{
			BeginMode3D(m_camera.GetCamera());
			//DrawModel(model, Vector3{ (float)0, 0.0f, (float)0 }, 0.5f, WHITE); // это проверка размера меша
			m_camera.ExtractFrustum();
			//DrawModel(model2, Vector3{ (float)0, 0.0f, (float)0 }, 1.0f, WHITE);
	
			// почему в разных циклах? чтобы не ломать батчинг, так как рейлиб не сортирует свою геометрию (кстати, меши не батчатся)
			
			// floor - grass
			for (int x = 0; x < MapSize; x++)
			{
				for (int y = 0; y < MapSize; y++)
				{
					const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
					const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

					if (m_world.openworld.tiles[x][y].type == TileType::Grass /* && m_camera.GetFrustum().AABBoxIn(min, max)*/)
					{
						DrawCubeTexture(m_resourceMgr.textureGrass, Vector3{ (float)x, -0.5f, (float)y }, 1, 1, 1, WHITE);
						//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
					}
				}
			}

			// floor - road
			for (int x = 0; x < MapSize; x++)
			{
				for (int y = 0; y < MapSize; y++)
				{
					const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
					const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

					if (m_world.openworld.tiles[x][y].type == TileType::Road && m_camera.GetFrustum().AABBoxIn(min, max))
					{
						DrawCubeTexture(m_resourceMgr.textureRoad, Vector3{ (float)x, -0.5f, (float)y }, 1, 1, 1, WHITE);
						//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);
					}
				}
			}

			// tree
			for (int x = 0; x < MapSize; x++)
			{
				for (int y = 0; y < MapSize; y++)
				{
					const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
					const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

					if (m_world.openworld.tiles[x][y].decor == TileDecorType::Tree)
					{
						if (m_camera.GetFrustum().AABBoxIn(min, max))
						{
							DrawBillboard(m_camera.GetCamera(), m_resourceMgr.textureTree, Vector3{ (float)x, 1.0f, (float)y }, 2.0f, WHITE);
							//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
							//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
							//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
						}
					}
				}
			}

			// town
			for (int x = 0; x < MapSize; x++)
			{
				for (int y = 0; y < MapSize; y++)
				{
					const Vector3 min = { x - 0.5f, 0.0f, y - 0.5f };
					const Vector3 max = { x + 0.5f, 1.0f, y + 0.5f };

					if (m_world.openworld.tiles[x][y].decor == TileDecorType::Town)
					{
						if (m_camera.GetFrustum().AABBoxIn(min, max))
						{
							DrawBillboard(m_camera.GetCamera(), m_resourceMgr.textureTown, Vector3{ (float)x, 1.0f, (float)y }, 2.0f, WHITE);
							//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
							//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
							//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
						}
					}
				}
			}

			EndMode3D();
		}

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

		//// Model map
		//{
		//	BeginMode3D(m_camera.GetCamera());
		//	DrawModel(model2, Vector3{ (float)0, 0.0f, (float)0 }, 1.0f, WHITE);
		//	EndMode3D();
		//}

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