#pragma once

#include "IGameModule.h"
#include "Global.h"

class Overworld final : public IGameModule, public IPlayfield
{
	static const int TileTypes = 128;
	static const int RowCount = 256;
	static const int ColCount = 256;
	static const int TileWidth = 16;
	static const int TileHeight = 16;
	static const int VisibleRows = 16;
	static const int VisibleCols = 17;
	static const int MiddleRow = 7;
	static const int MiddleCol = 7;
	static const int EnterTeleports = 32;

	typedef void (Overworld::* UpdateFunc)();

	static Overworld* instance;

	// The whole map is stored as a collection of rows, each individually compressed
	// Each uncompressed cell or element is the number of a unique tile (a tile reference)

	Table<uint8_t, RowCount> compressedRows;

	// Rows must be uncompressed to get the tile references. 
	// Uncompress the rows that are visible into a buffer.
	// 15 rows are visible at a time, if they're aligned with the screen. Otherwise, there are 16.

	uint8_t tileRefs[VisibleRows][ColCount];

	// The tile graphics are laid out as a grid of 16 tiles in each row
	// Going left to right, top to bottom, the tiles are in the tile reference order

	std::shared_ptr<Texture> tiles;

	uint16_t tileAttr[TileTypes];

	uint8_t tileBackdrops[TileTypes];

	LTeleport enterTeleports[EnterTeleports];

	// Keep track of the alignment of the visible region relative to the screen

	int     offsetX;
	int     offsetY;

	// In order to avoid uncompressing rows that are already in the buffer, 
	// keep track of where the top row is in the uncompressed buffer

	int     uncompStartRow;

	int     topRow;
	int     leftCol;

	std::shared_ptr<Texture> playerImage;

	int movingSpeed;
	Direction movingDir;
	UpdateFunc curUpdate;

	bool shopPending;
	int shopId;

	bool poisonMove;

public:
	Overworld() noexcept;
	~Overworld();

	void Init(int startCol, int startRow) noexcept;

	void Update() noexcept final;
	void Draw() noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	Point2 GetCurrentPos() noexcept final;
	int GetInRoom() noexcept final;

	void HandleShopClosed() noexcept final;
	void HandleFightEnded() noexcept final;
	void HandleOpeningEnded() noexcept final;

	static IGameModule* Make() noexcept;

	static bool IsActive();
	static uint16_t GetCurrentTileAttr();
	static Point2 GetPlayerPos() noexcept;

private:
	void LoadMap(int middleCol, int middleRow);
	void ShiftMap(int shiftX, int shiftY);

	void DrawMap();
	void DrawPlayer();
	void DrawVehicles();
	void DrawImage(int col, int row, int imageCol, int imageRow);
	void DrawBridge();
	void DrawIsmus();

	int GetFacingTileRef(Direction direction);
	int GetTileRef(int col, int row);

	Point2 GetPlayerRowCol();
	Point2 GetFacingRowCol(Direction direction);

	void UpdateFootIdle();
	void UpdateCanoeIdle();
	void UpdateShipIdle();
	void UpdateAirshipIdle();
	void UpdateMoving();
	void UpdateLift();
	void UpdateLand();

	bool CanWalk(uint16_t attrs);

	bool GetTriggeredTeleport(int& teleportId);
	bool GetTriggeredBattle(int& formationId);
};