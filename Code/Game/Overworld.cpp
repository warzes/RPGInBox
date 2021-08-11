#include "stdafx.h"
#include "Overworld.h"
#include "Sound.h"
#include "GameScenes.h"
#include "Player.h"
#include "MapSprite.h"
#include "VehicleSprites.h"
#include "OWTile.h"
#include <Engine/DebugNew.h>

Overworld* Overworld::instance;

IMapSprite* vehicleSprite;
MapSprite* playerSprite;
CanoeSprite* canoeSprite;
ShipSprite* shipSprite;
AirshipSprite* airshipSprite;

const int IsmusCol = 102;
const int IsmusRow = 164;
const int BridgeCol = 152;
const int BridgeRow = 152;

Overworld::Overworld() noexcept
	: tiles(nullptr)
	, offsetX(0)
	, offsetY(0)
	, uncompStartRow(0)
	, topRow(0)
	, leftCol(0)
	, playerImage(nullptr)
	, movingDir(Dir_None)
	, curUpdate(&Overworld::UpdateFootIdle)
	, shopPending(false)
	, shopId(0)
	, poisonMove(false)
{
	instance = this;
}

Overworld::~Overworld()
{
	instance = nullptr;
	delete playerSprite;
	playerSprite = nullptr;

	delete canoeSprite;
	canoeSprite = nullptr;

	delete shipSprite;
	shipSprite = nullptr;

	delete airshipSprite;
	airshipSprite = nullptr;
}

IPlayfield* Overworld::AsPlayfield() noexcept
{
	return this;
}

Point2 Overworld::GetCurrentPos() noexcept
{
	return GetPlayerRowCol();
}

int Overworld::GetInRoom() noexcept
{
	return 0;
}

void Overworld::HandleShopClosed() noexcept
{
	GameScenes::BeginFade(15, [] {});
	SoundManager::PlayTrack(Sound_Field);
}

void Overworld::HandleFightEnded() noexcept
{
	GameScenes::BeginFade(15, [] {});
	SoundManager::PlayTrack(Sound_Field);
}

void Overworld::HandleOpeningEnded() noexcept
{
	GameScenes::BeginFade(60, [] {});
	SoundManager::PlayTrack(Sound_Field);
}

void Overworld::Init(int startCol, int startRow) noexcept
{
	if (!LoadResource("owMap.tab", &compressedRows))
		return;

	tiles = ResourceManager::GetGlobalTexture("images/owTiles.png");
	playerImage = ResourceManager::GetGlobalTexture("images/mapPlayer.png");

	if (!LoadList("owTileAttr.dat", tileAttr, TileTypes))
		return;

	if (!LoadList("tileBackdrops.dat", tileBackdrops, TileTypes))
		return;

	if (!LoadList("enterTeleports.dat", enterTeleports, EnterTeleports))
		return;

	LoadMap(startCol, startRow);

	playerSprite = new MapSprite(playerImage);
	playerSprite->SetX(startCol * TileWidth);
	playerSprite->SetY(startRow * TileHeight);
	playerSprite->SetFrames(0 * 16);
	playerSprite->SetDirection(Dir_Down);

	canoeSprite = new CanoeSprite(playerImage);

	shipSprite = new ShipSprite(playerImage);

	airshipSprite = new AirshipSprite(playerImage);

	uint8_t ref = GetTileRef(startCol, startRow);
	uint16_t attrs = tileAttr[ref];

	if (OWTile::CanCanoe(attrs))
	{
		Player::SetActiveVehicle(Vehicle_Canoe);

		canoeSprite->SetDirection(Dir_Right);
		canoeSprite->SetX(playerSprite->GetX());
		canoeSprite->SetY(playerSprite->GetY());
		vehicleSprite = canoeSprite;

		curUpdate = &Overworld::UpdateCanoeIdle;
	}
	else
	{
		vehicleSprite = playerSprite;
	}

	SoundManager::PlayTrack(Sound_Field);

	GameScenes::BeginFade(15, [] {});
}

void Overworld::Update() noexcept
{
	// TODO:
}

void Overworld::Draw() noexcept
{
	// TODO:
}

bool Overworld::IsActive()
{
	return instance != nullptr;
}

Point2 Overworld::GetPlayerPos() noexcept
{
	if (instance == nullptr)
		return Point2();

	return instance->GetCurrentPos();
}

Point2 Overworld::GetPlayerRowCol()
{
	int col = (uint8_t)(leftCol + MiddleCol);
	int row = (uint8_t)(topRow + MiddleRow);
	Point2 pos = { col, row };

	return pos;
}

Point2 Overworld::GetFacingRowCol(Direction direction)
{
	int shiftCol = 0;
	int shiftRow = 0;

	switch (direction)
	{
	case Dir_Right: shiftCol = 1; break;
	case Dir_Left:  shiftCol = -1; break;
	case Dir_Down:  shiftRow = 1; break;
	case Dir_Up:    shiftRow = -1; break;
	}

	Point2 pos = GetPlayerRowCol();
	pos.x += shiftCol;
	pos.y += shiftRow;

	return pos;
}

void Overworld::UpdateFootIdle()
{
	// TODO:
}