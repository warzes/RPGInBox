#pragma once

typedef enum {
	CREATURE_STAT_LEVEL,
	CREATURE_STAT_EXPERIENCE,
	CREATURE_STAT_HEALTH,
	// Size
	CREATURE_STAT__LENGTH
} CreatureStat;

#include "Compass.h"
#include "Handle.h"

void InitCreatures(int capacity);
int MaxCreatures(void);

bool IsCreatureValid(Handle creature);
Handle SpawnCreature(void); // Requires a sprite and inventory to be inventory
void DestroyCreature(Handle creature);
void DestroyAllCreatures(void);
void DestroyAllButProtagonistCreatures(void);
int CountCreatures(void);

Handle GetCreatureAtPosition(Vector2 position);
Handle GetCreatureInventory(Handle creature); // Managed return. Never null
Vector2 GetCreaturePosition(Handle creature);
Handle GetCreatureSprite(Handle creature); // Managed return. Never null
int GetCreatureStat(Handle creature, CreatureStat stat);
void SetCreaturePosition(Handle creature, Vector2 position);
void SetCreatureStat(Handle creature, CreatureStat stat, int value);

Handle GetCreatureProtagonist(void);
bool IsCreatureProtagonist(Handle creature);
void SetCreatureProtagonist(Handle creature);
void UpdateCreatureProtagonistLevel(void);

void CreatureAttack(Handle creature, Compass direction);
void CreatureCloseDoor(Handle creature, Compass direction);
void CreatureOpenDoor(Handle creature, Compass direction);
void CreaturePickUpFloorItem(Handle creature, int inventoryIndex);
void CreatureWalk(Handle creature, Compass direction);
void CreatureWalkOrInteract(Handle creature, Compass direction);
void KillCreature(Handle creature);