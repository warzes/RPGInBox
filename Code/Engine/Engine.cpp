#include "stdafx.h"
#include "Engine.h"
#include "DebugNew.h"

#define SKYTEST 1

#if SKYTEST
Camera camera = { 0 };
Texture cubeTx;
Model cube;
Model mountain ;
Texture mountTx;
Model sky;
Texture skyTx;
Shader shaderS;
Shader shaderM;
int frameLoc;
float frame = 0;
#else

Camera camera = { { 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
Image imMap;      // Load cubicmap image (RAM)
Texture2D cubicmap;       // Convert image to texture to display (VRAM)
Mesh mesh;
Model model;

// NOTE: By default each cube is mapped to one part of texture atlas
Texture2D texture;    // Load map texture

// Get map image data to be used for collision detection
Color* mapPixels;
Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };  // Set model position
#endif

//-----------------------------------------------------------------------------
Engine::Engine() noexcept
{
}
//-----------------------------------------------------------------------------
Engine::~Engine()
{
#if SKYTEST
	UnloadModel(cube);
	UnloadModel(sky);
	UnloadModel(mountain);
	UnloadTexture(skyTx);
	UnloadTexture(mountTx);
	UnloadTexture(cubeTx);
	UnloadShader(shaderS);
	UnloadShader(shaderM);
#else
	UnloadImageColors(mapPixels);   // Unload color array

	UnloadTexture(cubicmap);        // Unload cubicmap texture
	UnloadTexture(texture);         // Unload map texture
	UnloadModel(model);             // Unload map model
#endif
	CloseWindowRaylib();
	log.Print("Engine end.");
}
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineConfig& config) noexcept
{
#if SE_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	if (!log.open(config.LogName))
		return false;

	log.Print("Engine Init...");

	InitWindow(config.window.width, config.window.height, "Game");
	SetTargetFPS(60);

#if SKYTEST
	// Define the camera to look into our 3d world
	camera.position = Vector3{ 5.0f, 1.0f, 5.1f };
	camera.target = Vector3{ 0.0f, .9f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	// texture and shade a models

	cubeTx = LoadTexture("../resources/test.png");
	cube = LoadModelFromMesh(GenMeshCube(.6, .6, .6));
	cube.materials[0].maps[MAP_DIFFUSE].texture = cubeTx;

	mountain = LoadModel("../resources/mountain.obj");
	mountTx = LoadTexture("../resources/mountain.png");
	mountain.materials[0].maps[MAP_DIFFUSE].texture = mountTx;

	sky = LoadModel("../resources/sky.obj");
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

#else
	imMap = LoadImage("../resources/cubicmap.png");      // Load cubicmap image (RAM)
	cubicmap = LoadTextureFromImage(imMap);       // Convert image to texture to display (VRAM)
	mesh = GenMeshCubicmap(imMap, Vector3{ 1.0f, 1.0f, 1.0f });
	model = LoadModelFromMesh(mesh);

	// NOTE: By default each cube is mapped to one part of texture atlas
	texture = LoadTexture("../resources/cubicmap_atlas.png");    // Load map texture
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;             // Set map diffuse texture

	// Get map image data to be used for collision detection
	mapPixels = LoadImageColors(imMap);
	UnloadImage(imMap);             // Unload image from RAM

	SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set camera mode
#endif
	//if (!window.init(config.window))
	//	return false;

	//if (!renderer.init())
	//	return false;

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Update() noexcept
{
	//window.update();
	//if (window.IsResize())
	//{
	//	renderer.resize();
	//}

	//BeginDrawing();

	//ClearBackground(RAYWHITE);

	//DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	//EndDrawing();
#if SKYTEST
	frame += 0.00004;

	UpdateCamera(&camera);

	SetShaderValue(shaderS, frameLoc, &frame, SHADER_UNIFORM_FLOAT);

	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();

	ClearBackground({ 0, 60, 20, 0 });

	BeginMode3D(camera);

	for (int i = -64; i < 64; i = i + 2)
	{
		DrawModel(cube, Vector3 { (float)i, .4, 0 }, 1, WHITE);
		DrawModel(cube, Vector3 { 0, .4, (float)i }, 1, WHITE);
	}

	DrawModel(mountain, camera.position, 1, WHITE);
	DrawModel(sky, camera.position, 1, WHITE);
	DrawGrid(20, 1.0f);        // Draw a grid


	EndMode3D();

	DrawFPS(10, 10);

	//int l = MeasureText(FormatText("Frame %d", frame), 20);
	//DrawRectangle(16, 698, l + 8, 42, BLUE);
	//DrawText(FormatText("Frame %d", frame), 20, 700, 20, WHITE);

	EndDrawing();
#else
	Vector3 oldCamPos = camera.position;    // Store old camera position

	UpdateCamera(&camera);      // Update camera

	// Check player collision (we simplify to 2D collision detection)
	Vector2 playerPos = { camera.position.x, camera.position.z };
	float playerRadius = 0.1f;  // Collision radius (player is modelled as a cilinder for collision)

	int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
	int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);

	// Out-of-limits security check
	if (playerCellX < 0) playerCellX = 0;
	else if (playerCellX >= cubicmap.width) playerCellX = cubicmap.width - 1;

	if (playerCellY < 0) playerCellY = 0;
	else if (playerCellY >= cubicmap.height) playerCellY = cubicmap.height - 1;

	// Check map collisions using image data and player position
	// TODO: Improvement: Just check player surrounding cells for collision
	for (int y = 0; y < cubicmap.height; y++)
	{
		for (int x = 0; x < cubicmap.width; x++)
		{
			if ((mapPixels[y * cubicmap.width + x].r == 255) &&       // Collision: white pixel, only check R channel
				(CheckCollisionCircleRec(playerPos, playerRadius,
					Rectangle {
				mapPosition.x - 0.5f + x * 1.0f, mapPosition.z - 0.5f + y * 1.0f, 1.0f, 1.0f
			})))
			{
				// Collision detected, reset camera position
				camera.position = oldCamPos;
			}
		}
	}
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();

	ClearBackground(RAYWHITE);

	BeginMode3D(camera);
	DrawModel(model, mapPosition, 1.0f, WHITE);                     // Draw maze map
	EndMode3D();

	DrawTextureEx(cubicmap, Vector2 { GetScreenWidth() - cubicmap.width * 4.0f - 20, 20.0f }, 0.0f, 4.0f, WHITE);
	DrawRectangleLines(GetScreenWidth() - cubicmap.width * 4 - 20, 20, cubicmap.width * 4, cubicmap.height * 4, GREEN);

	// Draw player position radar
	DrawRectangle(GetScreenWidth() - cubicmap.width * 4 - 20 + playerCellX * 4, 20 + playerCellY * 4, 4, 4, RED);

	DrawFPS(10, 10);

	EndDrawing();
#endif
}
//-----------------------------------------------------------------------------
void Engine::BeginFrame() noexcept
{
	//renderer.beginFrame();
}
//-----------------------------------------------------------------------------
void Engine::EndFrame() noexcept
{
	//renderer.endFrame();
}
//-----------------------------------------------------------------------------
bool Engine::IsEnd() const noexcept
{
	//return window.IsClose();
	return WindowShouldClose();
}
//-----------------------------------------------------------------------------