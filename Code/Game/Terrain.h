#pragma once

typedef enum TerrainType {
	TERRAIN_TYPE_GRASS,
	TERRAIN_TYPE_STONE,
	TERRAIN_TYPE_PUDDLE,
	// Size
	TERRAIN_TYPE__LENGTH
} TerrainType;

void InitTerrain(int width, int height, int depth);
int GetTerrainWidth(void);
int GetTerrainHeight(void);
int GetTerrainDepth(void);

void LoadDefaulTerrainTiles(void);

void ClearTerrain(TerrainType type, bool travelEnabled, int travelCost);
int GetTerrainTravelCost(int x, int y, int depth);
bool GetTerrainTravelEnabled(int x, int y, int depth);
TerrainType GetTerrainType(int x, int y, int depth);
TerminalTile GetTerrainTypeTile(TerrainType type);
bool IsTerrainTraversable(int x, int y, int depth);
bool IsWithinTerrain(int x, int y, int depth);
void SetTerrainTravelCost(int x, int y, int depth, int travelCost);
void SetTerrainTravelEnabled(int x, int y, int depth, bool enabled);
void SetTerrainType(int x, int y, int depth, TerrainType type);
void SetTerrainTypeTile(TerrainType type, TerminalTile tile);

void RenderTerrain(int depth);