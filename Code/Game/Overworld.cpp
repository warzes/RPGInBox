#include "stdafx.h"
#include "Overworld.h"
#include "Sound.h"
#include "GameScenes.h"

Overworld* Overworld::instance;

Overworld::Overworld() noexcept
	: tiles(nullptr),
	offsetX(0),
	offsetY(0),
	uncompStartRow(0),
	topRow(0),
	leftCol(0),
	playerImage(nullptr),
	movingDir(Dir_None),
	curUpdate(&Overworld::UpdateFootIdle),
	shopPending(false),
	shopId(0),
	poisonMove(false)
{
	instance = this;
}

Overworld::~Overworld()
{
	instance = nullptr;
	// TODO:
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
	// TODO:
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