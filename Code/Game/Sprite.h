#pragma once
#if ASCII_DEFINE
#include "Handle.h"

typedef enum SpriteLayer {
	SPRITE_LAYER_CONTAINER,
	SPRITE_LAYER_CREATURE,
	SPRITE_LAYER_DOOR,
	SPRITE_LAYER_PROTAGONIST,
	// Size
	SPRITE_LAYER__LENGTH
} SpriteLayer;

void InitSprites(int capacity);
int MaxSprites(void);

bool IsSpriteValid(Handle sprite);
Handle SpawnSprite(void);
void DestroySprite(Handle sprite);
void DestroyAllSprites(void);
int CountSprites(void);

bool GetSpriteEnabled(Handle sprite);
SpriteLayer GetSpriteLayer(Handle sprite);
Vector2 GetSpritePosition(Handle sprite);
TerminalTile GetSpriteTile(Handle sprite);
void SetSpriteEnabled(Handle sprite, bool enabled);
void SetSpriteLayer(Handle sprite, SpriteLayer layer);
void SetSpritePosition(Handle sprite, Vector2 position);
void SetSpriteTile(Handle sprite, TerminalTile tile);

int GetSpriteLayerPriority(SpriteLayer spriteLayer);
bool IsSpriteLayerEnabled(SpriteLayer spriteLayer);
void LoadDefaultSpriteLayers(void);
void SetSpriteLayerEnabled(SpriteLayer spriteLayer, bool enabled);
void SetSpriteLayerPriority(SpriteLayer spriteLayer, int priority);

void RenderSprites(void);
#endif