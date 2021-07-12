#include "stdafx.h"
#include "Creature.h"
#include "Container.h"
#include "Door.h"
#include "Game.h"
#include "Inventory.h"
#include "Item.h"
#include "Prompt.h"
#include "Sprite.h"

static void CacheActiveCreatures(int* outLength, Handle* outHandles[]);

static int creatureCapacity;
static int creatureCount;
static int creatureLowestFree;
static Handle creatureProtagonist;
static struct CreatureStatus {
	bool reserved;
	unsigned int version;
} *creatureStatus;
static struct CreatureData {
	Vector2 position;
	Handle sprite;
	Handle inventory;
	int stats[CREATURE_STAT__LENGTH];
} *creatureData;

void InitCreatures(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	creatureStatus = (CreatureStatus*)MemAlloc(capacity * sizeof(*creatureStatus));
	creatureData = (CreatureData*)MemAlloc(capacity * sizeof(*creatureData));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!creatureStatus || !creatureData) {
		TraceLog(LOG_ERROR, TextFormat("CREATURE: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	creatureCapacity = capacity;

	for (i = 0; i < creatureCapacity; i++) {
		creatureStatus[i].reserved = false;
		creatureStatus[i].version = 0;
	}

	for (i = 0; i < creatureCapacity; i++) {
		creatureData[i].sprite = NULL_HANDLE;
		creatureData[i].inventory = NULL_HANDLE;
	}

	creatureCount = 0;
	creatureLowestFree = 0;
	TraceLog(LOG_INFO, TextFormat("CREATURE: Initialized successfully (capacity: %d)", creatureCapacity));
}

int MaxCreatures(void)
{
	return creatureCapacity;
}

bool IsCreatureValid(Handle creature)
{
	return !IsHandleNull(creature)
		&& creatureStatus[creature.index].reserved
		&& creatureStatus[creature.index].version == creature.version;
}

Handle SpawnCreature(void)
{
	int i;

	for (i = creatureLowestFree; i < creatureCapacity; i++) {
		if (!creatureStatus[i].reserved) {
			int j;
			Handle handle;

			// Update instance status
			creatureStatus[i].reserved = true;
			creatureStatus[i].version++;

			// Make handle
			handle.version = creatureStatus[i].version;
			handle.index = i;

			// Update module status
			creatureCount++;
			creatureLowestFree = i + 1;

			// Prepare instance
			creatureData[i].position.x = 0;
			creatureData[i].position.y = 0;
			creatureData[i].sprite = SpawnSprite();
			creatureData[i].inventory = SpawnInventory();

			// EXIT: Sprite required
			if (!IsSpriteValid(creatureData[i].sprite)) {
				TraceLog(LOG_WARNING, TextFormat("CREATURE: Failed to spawn because there are not enough sprites (%d/%d)", CountSprites(), MaxSprites()));
				DestroyCreature(handle);
				return NULL_HANDLE;
			}

			// EXIT: Inventory required
			if (!IsInventoryValid(creatureData[i].inventory)) {
				TraceLog(LOG_WARNING, TextFormat("CREATURE: Failed to spawn because there are not enough inventories (%d/%d)", CountInventories(), MaxInventories()));
				DestroyCreature(handle);
				return NULL_HANDLE;
			}

			// Prepare instance (continued)
			for (j = 0; j < CREATURE_STAT__LENGTH; j++)
				creatureData[i].stats[j] = 0;

			SetSpriteLayer(creatureData[i].sprite, SPRITE_LAYER_CREATURE);

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroyCreature(Handle creature)
{
	creatureStatus[creature.index].reserved = false;

	if (IsSpriteValid(creatureData[creature.index].sprite)) {
		DestroySprite(creatureData[creature.index].sprite);
		creatureData[creature.index].sprite = NULL_HANDLE;
	}

	creatureCount--;

	if (creature.index < creatureLowestFree)
		creatureLowestFree = creature.index;
}

void DestroyAllCreatures(void)
{
	int i;

	for (i = 0; i < creatureCapacity; i++) {
		creatureStatus[i].reserved = false;

		if (IsSpriteValid(creatureData[i].sprite)) {
			DestroySprite(creatureData[i].sprite);
			creatureData[i].sprite = NULL_HANDLE;
		}
	}

	creatureCount = 0;
	creatureLowestFree = 0;
}

void DestroyAllButProtagonistCreatures(void)
{
	int i;

	// EXIT: If there is no protagonist, do standard destroy all creatures
	if (!IsCreatureValid(creatureProtagonist)) {
		DestroyAllCreatures();
		return;
	}

	// Destroy all creatures with indexes lower than protagonist
	for (i = 0; i < creatureProtagonist.index; i++) {
		creatureStatus[i].reserved = false;

		if (IsSpriteValid(creatureData[i].sprite))
			DestroySprite(creatureData[i].sprite);
	}

	// Destroy all creatures with indexes higher than protagonist
	for (i = creatureProtagonist.index + 1; i < creatureCapacity; i++) {
		creatureStatus[i].reserved = false;

		if (IsSpriteValid(creatureData[i].sprite))
			DestroySprite(creatureData[i].sprite);
	}

	creatureCount = 1;
	creatureLowestFree = (creatureProtagonist.index > 0) ? 0 : 1;
}

int CountCreatures(void)
{
	return creatureCapacity;
}

Handle GetCreatureAtPosition(Vector2 position)
{
	int i;
	int length;
	Handle found;
	Handle* cache;

	CacheActiveCreatures(&length, &cache);

	if (!cache)
		return NULL_HANDLE;

	found = NULL_HANDLE;

	for (i = 0; i < length; i++) {
		if (creatureData[cache[i].index].position.x == position.x
			&& creatureData[cache[i].index].position.y == position.y) {
			found = cache[i];
			break;
		}
	}

	MemFree(cache);
	return found;
}

Handle GetCreatureInventory(Handle creature)
{
	return creatureData[creature.index].inventory;
}

Vector2 GetCreaturePosition(Handle creature)
{
	return creatureData[creature.index].position;
}

Handle GetCreatureSprite(Handle creature)
{
	return creatureData[creature.index].sprite;
}

int GetCreatureStat(Handle creature, CreatureStat stat)
{
	return creatureData[creature.index].stats[stat];
}

void SetCreaturePosition(Handle creature, Vector2 position)
{
	creatureData[creature.index].position = position;
	SetSpritePosition(creatureData[creature.index].sprite, position);
}

void SetCreatureStat(Handle creature, CreatureStat stat, int value)
{
	creatureData[creature.index].stats[stat] = value;
}

Handle GetCreatureProtagonist(void)
{
	return creatureProtagonist;
}

bool IsCreatureProtagonist(Handle creature)
{
	return IsCreatureValid(creatureProtagonist)
		&& AreHandlesEqual(creature, creatureProtagonist);
}

void SetCreatureProtagonist(Handle creature)
{
	if (IsCreatureValid(creatureProtagonist))
		SetSpriteLayer(creatureData[creatureProtagonist.index].sprite, SPRITE_LAYER_CREATURE);

	creatureProtagonist = creature;

	if (IsCreatureValid(creature))
		SetSpriteLayer(creatureData[creature.index].sprite, SPRITE_LAYER_PROTAGONIST);
}

void UpdateCreatureProtagonistLevel(void)
{
	Handle protagonist;
	int currentLevel;
	int currentExp;
	int requiredExp;

	protagonist = GetCreatureProtagonist();
	currentLevel = GetCreatureStat(protagonist, CREATURE_STAT_LEVEL);
	currentExp = GetCreatureStat(protagonist, CREATURE_STAT_EXPERIENCE);
	requiredExp = currentLevel + 1; // TODO: Good enough for now, but will scale poorly

	if (currentExp >= requiredExp) {
		currentExp -= requiredExp;
		currentLevel++;
		SetCreatureStat(protagonist, CREATURE_STAT_LEVEL, currentLevel);
		SetCreatureStat(protagonist, CREATURE_STAT_EXPERIENCE, currentExp);
		OpenLevelUpPrompt(currentLevel);
	}
}

void CreatureAttack(Handle creature, Compass direction)
{
	Handle obstacle;
	Vector2 destination;

	destination = Vector2Add(GetCreaturePosition(creature), CompassToVector2(direction));
	obstacle = GetCreatureAtPosition(destination);

	if (IsCreatureValid(obstacle)) {
		int damage;
		int health;

		damage = GetRandomValue(1, 6);
		health = GetCreatureStat(obstacle, CREATURE_STAT_HEALTH);
		health -= damage;
		SetCreatureStat(obstacle, CREATURE_STAT_HEALTH, health);

		if (IsCreatureProtagonist(creature))
			TraceLog(LOG_INFO, TextFormat("CREATURE: Protagonist dealt %d damage to creature", damage));

		if (health < 0) {
			KillCreature(obstacle);

			if (IsCreatureProtagonist(creature)) {
				TraceLog(LOG_INFO, "CREATURE: Protagonist killed creature");
				SetCreatureStat(creature, CREATURE_STAT_EXPERIENCE, GetCreatureStat(creature, CREATURE_STAT_EXPERIENCE) + 1);
				UpdateCreatureProtagonistLevel();
			}
		}
	}
}

void CreatureCloseDoor(Handle creature, Compass direction)
{
	Handle obstacle;
	Vector2 destination;

	destination = Vector2Add(GetCreaturePosition(creature), CompassToVector2(direction));
	obstacle = GetDoorAtPosition(destination);

	if (IsDoorValid(obstacle)) {
		if (IsDoorOpened(obstacle)) {
			SetDoorOpened(obstacle, false);

			if (IsCreatureProtagonist(creature))
				TraceLog(LOG_INFO, "CREATURE: Protagonist closed a door");
		}
	}
}

void CreatureOpenDoor(Handle creature, Compass direction)
{
	Handle obstacle;
	Vector2 destination;

	destination = Vector2Add(GetCreaturePosition(creature), CompassToVector2(direction));
	obstacle = GetDoorAtPosition(destination);

	if (IsDoorValid(obstacle)) {
		if (!IsDoorOpened(obstacle)) {
			SetDoorOpened(obstacle, true);

			if (IsCreatureProtagonist(creature))
				TraceLog(LOG_INFO, "CREATURE: Protagonist opened a door");
		}
	}
}

void CreaturePickUpFloorItem(Handle creature, int inventoryIndex)
{
	Handle container;
	Vector2 location;

	location = GetCreaturePosition(creature);
	container = GetContainerAtPosition(location);

	if (IsContainerValid(container)) {
		Handle containerInventory;
		Handle creatureInventory;
		ItemPrefab itemPrefab;

		creatureInventory = GetCreatureInventory(creature);
		containerInventory = GetContainerInventory(container);
		itemPrefab = GetInventoryItemAtIndex(containerInventory, inventoryIndex);

		if (itemPrefab != ITEM_PREFAB_NONE) {
			if (IsInventoryFull(creatureInventory)) {
				if (IsCreatureProtagonist(creature))
					TraceLog(LOG_INFO, "CREATURE: Protagonist's inventory is too full to pick up this item");
			}
			else {
				AddInventoryItem(creatureInventory, itemPrefab, 1);
				RemoveInventoryItem(containerInventory, itemPrefab, 1);
			}
		}
	}
}

void CreatureWalk(Handle creature, Compass direction)
{
	Handle obstacle;
	Vector2 destination;

	destination = Vector2Add(GetCreaturePosition(creature), CompassToVector2(direction));

	// Check for doors
	{
		obstacle = GetDoorAtPosition(destination);

		if (IsDoorValid(obstacle)) {
			if (!IsDoorOpened(obstacle)) {
				if (IsCreatureProtagonist(creature))
					TraceLog(LOG_INFO, "CREATURE: Protagonist walked into a door");

				return;
			}
		}
	}

	// Check for creatures
	{
		obstacle = GetCreatureAtPosition(destination);

		if (IsCreatureValid(obstacle)) {
			if (IsCreatureProtagonist(creature))
				TraceLog(LOG_INFO, "CREATURE: Protagonist walked into a creature");

			return;
		}
	}

	// No collision
	SetCreaturePosition(creature, destination);
}

// NOTE: This is inefficient. However, it should only be used by the protagonist
void CreatureWalkOrInteract(Handle creature, Compass direction)
{
	Vector2 destination;
	Handle obstacle;

	destination = Vector2Add(GetCreaturePosition(creature), CompassToVector2(direction));

	// Check for door collision
	obstacle = GetDoorAtPosition(destination);

	if (IsDoorValid(obstacle) && !IsDoorOpened(obstacle)) {
		CreatureOpenDoor(obstacle, direction);
		return;
	}

	// Check for creature collision
	obstacle = GetCreatureAtPosition(destination);

	if (IsCreatureValid(obstacle)) {
		CreatureAttack(creature, direction);
		return;
	}

	// No collision
	CreatureWalk(creature, direction);
}

void KillCreature(Handle creature)
{
	if (IsCreatureProtagonist(creature)) {
		TraceLog(LOG_INFO, "CREATURE: Protagonist died");
		GameOver();
	}
	else {
		// TODO: Drop carcass/loot
		DestroyCreature(creature);
	}
}

static void CacheActiveCreatures(int* outLength, Handle* outHandles[])
{
	int i;
	int j;

	// EXIT: There are no creatures
	if (creatureCount == 0) {
		*outLength = 0;
		*outHandles = NULL;
		return;
	}

	*outLength = 0;
	*outHandles = (Handle*)MemAlloc(creatureCount * sizeof(**outHandles));

	// CRASH: Allocation failure
	if (!*outHandles) {
		TraceLog(LOG_ERROR, TextFormat("CREATURE: Failed to allocate cache (size: %d)", creatureCount));
		return;
	}

	for (j = 0, i = 0; i < creatureCapacity; i++) {
		if (creatureStatus[i].reserved) {
			(*outHandles)[j].index = i;
			(*outHandles)[j].version = creatureStatus[i].version;
			j++;

			// BREAK: Leave early if we've found all the instances
			if (j == creatureCount)
				break;
		}
	}

	*outLength = j;
}
