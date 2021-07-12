#include "stdafx.h"
#include "Terrain.h"

// Basically means that travel costs 1 action/turn
#define TERRAIN_MIN_TRAVERSABLE_TRAVEL_COST 1

static int GetTerrainArea(void);
static int GetTerrainIndexFromXYZ(int x, int y, int z);

static int terrainWidth;
static int terrainHeight;
static int terrainDepth;
static struct Terrain {
	TerrainType type;
	bool travelEnabled;
	int travelCost;
} *terrain;
static TerminalTile terrainTile[TERRAIN_TYPE__LENGTH];

void InitTerrain(int width, int height, int depth)
{
	int i;

	// CRASH: Terrain must be at least a 1x1x1 cube
	if (width < 1 || height < 1 || depth < 1) {
		TraceLog(LOG_ERROR, TextFormat("TERRAIN: Failed to initialize terrain. Terrain must have a size of at least 1x1x1. Size was %dx%dx%d", width, height, depth));
		return;
	}

	terrainWidth = width;
	terrainHeight = height;
	terrainDepth = depth;
	terrain = (Terrain*)MemAlloc(width * height * depth * sizeof(*terrain));

	// CRASH: Allocation failure
	if (!terrain) {
		TraceLog(LOG_ERROR, TextFormat("TERRAIN: Failed to allocate module to the size of %dx%d%d (%d total)", width, height, depth, (width * height * depth)));
		return;
	}

	ClearTerrain(TERRAIN_TYPE_GRASS, true, TERRAIN_MIN_TRAVERSABLE_TRAVEL_COST);
	LoadDefaulTerrainTiles();
	TraceLog(LOG_INFO, TextFormat("TERRAIN: Initialized successfully (size: %dx%dx%d, total: %d)", width, height, depth, (width * height * depth)));
}

void ClearTerrain(TerrainType type, bool travelEnabled, int travelCost)
{
	int i;
	int length;

	length = GetTerrainArea();

	for (i = 0; i < length; i++) {
		terrain[i].type = type;
		terrain[i].travelEnabled = travelEnabled;
		terrain[i].travelCost = travelCost;
	}
}

inline int GetTerrainWidth(void)
{
	return terrainWidth;
}

inline int GetTerrainHeight(void)
{
	return terrainHeight;
}

inline int GetTerrainDepth(void)
{
	return terrainDepth;
}

void LoadDefaulTerrainTiles(void)
{
	terrainTile[TERRAIN_TYPE_GRASS] = TerminalTile{ .background = BLACK, .foreground = DARKGREEN, .symbol = ',' };
	terrainTile[TERRAIN_TYPE_STONE] = TerminalTile{ .background = BLACK, .foreground = DARKGRAY, .symbol = '.' };
	terrainTile[TERRAIN_TYPE_PUDDLE] = TerminalTile{ .background = BLACK, .foreground = BLUE, .symbol = '~' };
}

inline int GetTerrainTravelCost(int x, int y, int depth)
{
	return terrain[GetTerrainIndexFromXYZ(x, y, depth)].travelCost;
}

inline bool GetTerrainTravelEnabled(int x, int y, int depth)
{
	return terrain[GetTerrainIndexFromXYZ(x, y, depth)].travelEnabled;
}

inline TerrainType GetTerrainType(int x, int y, int depth)
{
	return terrain[GetTerrainIndexFromXYZ(x, y, depth)].type;
}

inline TerminalTile GetTerrainTypeTile(TerrainType type)
{
	return terrainTile[type];
}

inline bool IsTerrainTraversable(int x, int y, int depth)
{
	return GetTerrainTravelEnabled(x, y, depth)
		&& GetTerrainTravelCost(x, y, depth) >= TERRAIN_MIN_TRAVERSABLE_TRAVEL_COST;
}

bool IsWithinTerrain(int x, int y, int depth)
{
	return depth >= 0 && depth < terrainDepth
		&& y >= 0 && y < terrainHeight
		&& x >= 0 && x < terrainWidth;
}

inline void SetTerrainTravelCost(int x, int y, int depth, int travelCost)
{
	terrain[GetTerrainIndexFromXYZ(x, y, depth)].travelCost = travelCost;
}

inline void SetTerrainTravelEnabled(int x, int y, int depth, bool enabled)
{
	terrain[GetTerrainIndexFromXYZ(x, y, depth)].travelEnabled = enabled;
}

inline void SetTerrainType(int x, int y, int depth, TerrainType type)
{
	terrain[GetTerrainIndexFromXYZ(x, y, depth)].type = type;
}

inline void SetTerrainTypeTile(TerrainType type, TerminalTile tile)
{
	terrainTile[type] = tile;
}

void RenderTerrain(int depth)
{
	int x;
	int y;
	int width;
	int height;

	// CRASH: Depth is out of range
	if (depth < 0 || depth > terrainDepth) {
		TraceLog(LOG_ERROR, TextFormat("TERRAIN: Cannot render depth %d because it is out of bounds", depth));
		return;
	}

	width = (terrainWidth < GetTerminalWidth()) ? terrainWidth : GetTerminalWidth();
	height = (terrainHeight < GetTerminalHeight()) ? terrainHeight : GetTerminalHeight();

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++)
			SetTerminalTile(x, y, terrainTile[terrain[GetTerrainIndexFromXYZ(x, y, depth)].type]);
	}
}

inline static int GetTerrainArea(void)
{
	return terrainWidth * terrainHeight * terrainDepth;
}

inline static int GetTerrainIndexFromXYZ(int x, int y, int z)
{
	return (z * terrainWidth * terrainHeight) + (y * terrainWidth) + x;
}