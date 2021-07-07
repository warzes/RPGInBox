#include "stdafx.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
GameLogic::GameLogic(Engine& engine) noexcept
	: m_engine(engine)
{
}
//-----------------------------------------------------------------------------
GameLogic::~GameLogic()
{
	UnloadTexture(tx);
}
//-----------------------------------------------------------------------------
bool GameLogic::Init() noexcept
{
	img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
	tx = LoadTextureFromImage(img);
	UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	cam.Setup(45, Vector3{ 1, 0, 0 });
	cam.MoveSpeed.z = 10;
	cam.MoveSpeed.x = 5;
	cam.FarPlane = 5000;

	return true;
}
//-----------------------------------------------------------------------------
bool GameLogic::Update() noexcept
{
	cam.Update();
	return true;
}
//-----------------------------------------------------------------------------
void GameLogic::Frame() noexcept
{
	BeginDrawing();
	ClearBackground(WHITE);

	cam.BeginMode3D();
	//BeginMode3D(cam.GetCamera());
	//cam.ExtractFrustum();

	// grid of cube trees on a plane to make a "world"
	DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, PURPLE); // simple world plane
	float spacing = 4;
	int count = 5;

	int total = 0;
	int vis = 0;

	for (float x = -count * spacing; x <= count * spacing; x += spacing)
	{
		for (float z = -count * spacing; z <= count * spacing; z += spacing)
		{
			Vector3 pos = { x, 0.5f, z };

			Vector3 min = { x - 0.5f,0,z - 0.5f };
			Vector3 max = { x + 0.5f,1,z + 0.5f };
			total++;
			if (cam.GetFrustum().AABBoxIn(min, max))
			{
				DrawCubeTexture(tx, Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
				DrawCubeTexture(tx, Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
				vis++;
			}
		}
	}

	DrawGrid(20, spacing);         // Draw a grid
	
	cam.EndMode3D();
	//EndMode3D();

	DrawText(TextFormat("%d visible of %d total", vis, total), 200, 0, 20, GREEN);

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
