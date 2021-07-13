#include "stdafx.h"
#if ASCII_DEFINE
#include "Sprite.h"

static int spriteCapacity;
static int spriteCount;
static int spriteLowestFree;
static struct {
	bool enabled;
	int priority;
} spriteLayerData[SPRITE_LAYER__LENGTH];
static struct SpriteStatus {
	bool reserved;
	unsigned int version;
} *spriteStatus;
static struct SpriteData {
	bool enabled;
	Vector2 position;
	SpriteLayer layer;
	TerminalTile tile;
} *spriteData;

void InitSprites(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	spriteStatus = (SpriteStatus*)MemAlloc(capacity * sizeof(*spriteStatus));
	spriteData = (SpriteData*)MemAlloc(capacity * sizeof(*spriteData));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!spriteStatus || !spriteData) {
		TraceLog(LOG_ERROR, TextFormat("SPRITE: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	spriteCapacity = capacity;

	for (i = 0; i < spriteCapacity; i++) {
		spriteStatus[i].reserved = false;
		spriteStatus[i].version = 0;
	}

	LoadDefaultSpriteLayers();
	TraceLog(LOG_INFO, TextFormat("SPRITE: Initialized successfully (capacity: %d)", spriteCapacity));
}

int MaxSprites(void)
{
	return spriteCapacity;
}

bool IsSpriteValid(Handle sprite)
{
	return !IsHandleNull(sprite)
		&& spriteStatus[sprite.index].reserved
		&& spriteStatus[sprite.index].version == sprite.version;
}

Handle SpawnSprite(void)
{
	int i;

	for (i = spriteLowestFree; i < spriteCapacity; i++) {
		if (!spriteStatus[i].reserved) {
			Handle handle;

			// Update instance status
			spriteStatus[i].reserved = true;
			spriteStatus[i].version++;

			// Make handle
			handle.version = spriteStatus[i].version;
			handle.index = i;

			// Update module status
			spriteCount++;
			spriteLowestFree = i + 1;

			// Prepare instance
			spriteData[i].enabled = true;
			spriteData[i].position.x = 0;
			spriteData[i].position.y = 0;
			spriteData[i].layer = SPRITE_LAYER_CREATURE;
			spriteData[i].tile.background = ALPHA_BLACK;
			spriteData[i].tile.foreground = DARKGRAY;
			spriteData[i].tile.symbol = '?';

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroySprite(Handle sprite)
{
	spriteStatus[sprite.index].reserved = false;
	spriteCount--;

	if (sprite.index < spriteLowestFree)
		spriteLowestFree = sprite.index;
}

void DestroyAllSprites(void)
{
	int i;

	for (i = 0; i < spriteCapacity; i++)
		spriteStatus[i].reserved = false;

	spriteCount = 0;
	spriteLowestFree = 0;
}

int CountSprites(void)
{
	return spriteCount;
}

bool GetSpriteEnabled(Handle sprite)
{
	return spriteData[sprite.index].enabled;
}

SpriteLayer GetSpriteLayer(Handle sprite)
{
	return spriteData[sprite.index].layer;
}

Vector2 GetSpritePosition(Handle sprite)
{
	return spriteData[sprite.index].position;
}

TerminalTile GetSpriteTile(Handle sprite)
{
	return spriteData[sprite.index].tile;
}

void SetSpriteEnabled(Handle sprite, bool enabled)
{
	spriteData[sprite.index].enabled = enabled;
}

void SetSpriteLayer(Handle sprite, SpriteLayer layer)
{
	spriteData[sprite.index].layer = layer;
}

void SetSpritePosition(Handle sprite, Vector2 position)
{
	spriteData[sprite.index].position = position;
}

void SetSpriteTile(Handle sprite, TerminalTile tile)
{
	spriteData[sprite.index].tile = tile;
}

int GetSpriteLayerPriority(SpriteLayer spriteLayer)
{
	return spriteLayerData[spriteLayer].priority;
}

bool IsSpriteLayerEnabled(SpriteLayer spriteLayer)
{
	return spriteLayerData[spriteLayer].enabled;
}

void LoadDefaultSpriteLayers(void)
{
	spriteLayerData[SPRITE_LAYER_CONTAINER].enabled = true;
	spriteLayerData[SPRITE_LAYER_CONTAINER].priority = 90;

	spriteLayerData[SPRITE_LAYER_CREATURE].enabled = true;
	spriteLayerData[SPRITE_LAYER_CREATURE].priority = 80;

	spriteLayerData[SPRITE_LAYER_DOOR].enabled = true;
	spriteLayerData[SPRITE_LAYER_DOOR].priority = 10;

	spriteLayerData[SPRITE_LAYER_PROTAGONIST].enabled = true;
	spriteLayerData[SPRITE_LAYER_PROTAGONIST].priority = 100;
}

void SetSpriteLayerEnabled(SpriteLayer spriteLayer, bool enabled)
{
	spriteLayerData[spriteLayer].enabled = enabled;
}

void SetSpriteLayerPriority(SpriteLayer spriteLayer, int priority)
{
	spriteLayerData[spriteLayer].priority = priority;
}

void RenderSprites(void)
{
	int i;
	int j;
	int cacheLength;
	struct _cache {
		Handle current;
		Handle temporary;
	} *cache;

	cacheLength = spriteCount;
	cache = (_cache*)MemAlloc(spriteCount * sizeof(*cache));

	// CRASH: Allocation failure
	if (!cache) {
		TraceLog(LOG_ERROR, TextFormat("SPRITE: Render failed to allocate a handle cache (size: %d)", spriteCount));
		return;
	}

	// Cache active
	for (j = 0, i = 0; i < spriteCapacity; i++) {
		if (spriteStatus[i].reserved) {
			cache[j].current.index = i;
			cache[j].current.version = spriteStatus[i].version;
			j++;

			// BREAK: Leave when we have found all active instances
			if (j == spriteCapacity)
				break;
		}
	}

	cacheLength = j;

	// Cull disabled and sprites whose sprite layer is disabled
	for (j = 0, i = 0; i < cacheLength; i++) {
		if (spriteData[i].enabled && spriteLayerData[spriteData[i].layer].enabled) {
			cache[i].current = cache[j].current;
			j++;
		}
	}

	cacheLength = j;

	// Cull out of bounds
	for (j = 0, i = 0; i < cacheLength; i++) {
		if (IsWithinTerminalV(spriteData[i].position)) {
			cache[j].current = cache[i].current;
			j++;
		}
	}

	cacheLength = j;

	// Sort by layer (counting algorithm)
	{
		int layerCount[SPRITE_LAYER__LENGTH];
		int layerIndexStart[SPRITE_LAYER__LENGTH];
		SpriteLayer sortedLayers[SPRITE_LAYER__LENGTH];

		// Reset layer counts
		for (i = 0; i < SPRITE_LAYER__LENGTH; i++) {
			layerCount[i] = 0;
			layerIndexStart[i] = 0;
			sortedLayers[i] =(SpriteLayer) i;
		}

		// Count the occurences of each layer
		for (i = 0; i < cacheLength; i++) {
			int currentIndex;
			SpriteLayer currentLayer;

			currentIndex = cache[i].current.index;
			currentLayer = spriteData[currentIndex].layer;
			layerCount[currentLayer]++;
		}

		// Sort layers by priority (insertion sort)
		{
			SpriteLayer currentLayer;

			for (i = 1; i < SPRITE_LAYER__LENGTH; i++) {
				currentLayer = sortedLayers[i];
				j = i - 1;

				while (j >= 0 && spriteLayerData[sortedLayers[j]].priority > spriteLayerData[currentLayer].priority) {
					sortedLayers[j + 1] = sortedLayers[j];
					j = j - 1;
				}

				sortedLayers[j + 1] = currentLayer;
			}
		}

		// Obtain starting index positions for each layer
		layerIndexStart[sortedLayers[0]] = 0;
		j = 0;

		for (i = 1; i < SPRITE_LAYER__LENGTH; i++) {
			j += layerCount[sortedLayers[i - 1]];
			layerIndexStart[sortedLayers[i]] = j;
		}

		// Merge-sort current cache into temporary cache. Use layer 
		// start index as an incrementer for that layers position, 
		// which after all our loops, we are safe to assume will not 
		// go out of bounds
		for (i = 0; i < cacheLength; i++) {
			int currentIndex;
			int targetIndex;
			SpriteLayer currentLayer;

			currentIndex = cache[i].current.index;
			currentLayer = spriteData[currentIndex].layer;
			targetIndex = layerIndexStart[currentLayer];
			cache[targetIndex].temporary = cache[i].current;
			layerIndexStart[currentLayer]++;
		}

		// Merge the temporary cache into the current cache
		for (i = 0; i < cacheLength; i++)
			cache[i].current = cache[i].temporary;
	}

	// Render to terminal
	for (i = 0; i < cacheLength; i++)
		SetTerminalTileV(spriteData[cache[i].current.index].position, spriteData[cache[i].current.index].tile);
}
#endif