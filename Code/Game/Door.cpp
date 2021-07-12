#include "stdafx.h"
#include "Door.h"
#include "Sprite.h"

static void CacheActiveDoors(int* outLength, Handle* outHandles[]);

static int doorCapacity;
static int doorCount;
static int doorLowestFree;
static struct DoorStatus {
	bool reserved;
	unsigned int version;
} *doorStatus;
static struct DoorData {
	Vector2 position;
	bool opened;
	Handle sprite;
	TerminalTile openedTile;
	TerminalTile closedTile;
} *doorData;

void InitDoors(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	doorStatus = (DoorStatus*)MemAlloc(capacity * sizeof(*doorStatus));
	doorData = (DoorData*)MemAlloc(capacity * sizeof(*doorData));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!doorStatus || !doorData) {
		TraceLog(LOG_ERROR, TextFormat("DOOR: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	doorCapacity = capacity;

	for (i = 0; i < doorCapacity; i++) {
		doorStatus[i].reserved = false;
		doorStatus[i].version = 0;
	}

	for (i = 0; i < doorCapacity; i++)
		doorData[i].sprite = NULL_HANDLE;

	doorCount = 0;
	doorLowestFree = 0;
	TraceLog(LOG_INFO, TextFormat("DOOR: Initialized successfully (capacity: %d)", doorCapacity));
}

int MaxDoors(void)
{
	return doorCapacity;
}

bool IsDoorValid(Handle door)
{
	return !IsHandleNull(door)
		&& doorStatus[door.index].reserved
		&& doorStatus[door.index].version == door.version;
}

Handle SpawnDoor(void)
{
	int i;

	for (i = doorLowestFree; i < doorCapacity; i++) {
		if (!doorStatus[i].reserved) {
			int j;
			Handle handle;

			// Update instance status
			doorStatus[i].reserved = true;
			doorStatus[i].version++;

			// Make handle
			handle.version = doorStatus[i].version;
			handle.index = i;

			// Update module status
			doorCount++;
			doorLowestFree = i + 1;

			// Prepare instance
			doorData[i].position.x = 0;
			doorData[i].position.y = 0;
			doorData[i].opened = false;
			doorData[i].sprite = SpawnSprite();
			doorData[i].openedTile = TerminalTile{ .background = ALPHA_BLACK, .foreground = BROWN, .symbol = '/' };
			doorData[i].closedTile = TerminalTile{ .background = ALPHA_BLACK, .foreground = BROWN, .symbol = '+' };

			// EXIT: Sprite required
			if (!IsSpriteValid(doorData[i].sprite)) {
				TraceLog(LOG_WARNING, TextFormat("DOOR: Failed to spawn because there are not enough sprites (%d/%d)", CountSprites(), MaxSprites()));
				DestroyDoor(handle);
				return NULL_HANDLE;
			}

			// Prepare instance (continued)
			SetSpriteLayer(doorData[i].sprite, SPRITE_LAYER_DOOR);
			SetSpriteTile(doorData[i].sprite, (doorData[i].opened) ? doorData[i].openedTile : doorData[i].closedTile);

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroyDoor(Handle door)
{
	doorStatus[door.index].reserved = false;

	if (IsSpriteValid(doorData[door.index].sprite)) {
		DestroySprite(doorData[door.index].sprite);
		doorData[door.index].sprite = NULL_HANDLE;
	}

	doorCount--;

	if (door.index < doorLowestFree)
		doorLowestFree = door.index;
}

void DestroyAllDoors(void)
{
	int i;

	for (i = 0; i < doorCapacity; i++) {
		doorStatus[i].reserved = false;

		if (IsSpriteValid(doorData[i].sprite)) {
			DestroySprite(doorData[i].sprite);
			doorData[i].sprite = NULL_HANDLE;
		}
	}

	doorCount = 0;
	doorLowestFree = 0;
}

int CountDoors(void)
{
	return doorCount;
}

Handle GetDoorAtPosition(Vector2 position)
{
	int i;
	int length;
	Handle found;
	Handle* cache;

	CacheActiveDoors(&length, &cache);

	if (!cache)
		return NULL_HANDLE;

	found = NULL_HANDLE;

	for (i = 0; i < length; i++) {
		if (doorData[cache[i].index].position.x == position.x
			&& doorData[cache[i].index].position.y == position.y) {
			found = cache[i];
			break;
		}
	}

	MemFree(cache);
	return found;
}

TerminalTile GetDoorClosedTile(Handle door)
{
	return doorData[door.index].closedTile;
}

TerminalTile GetDoorOpenedTile(Handle door)
{
	return doorData[door.index].openedTile;
}

Vector2 GetDoorPosition(Handle door)
{
	return doorData[door.index].position;
}

void SetDoorClosedTile(Handle door, TerminalTile closedTile)
{
	doorData[door.index].closedTile = closedTile;

	// Only change the sprite tile if we are in the closed state
	if (!doorData[door.index].opened)
		SetSpriteTile(doorData[door.index].sprite, doorData[door.index].closedTile);
}

bool IsDoorOpened(Handle door)
{
	return doorData[door.index].opened;
}

void SetDoorOpenedTile(Handle door, TerminalTile openedTile)
{
	doorData[door.index].openedTile = openedTile;

	// Only change the sprite tile if we are in the opened state
	if (!doorData[door.index].opened)
		SetSpriteTile(doorData[door.index].sprite, doorData[door.index].openedTile);
}

void SetDoorOpened(Handle door, bool opened)
{
	doorData[door.index].opened = opened;
	SetSpriteTile(doorData[door.index].sprite, (doorData[door.index].opened) ? doorData[door.index].openedTile : doorData[door.index].closedTile);
}

void SetDoorPosition(Handle door, Vector2 position)
{
	doorData[door.index].position = position;
	SetSpritePosition(doorData[door.index].sprite, position);
}

static void CacheActiveDoors(int* outLength, Handle* outHandles[])
{
	int i;
	int j;

	// EXIT: There are no doors
	if (doorCount == 0) {
		*outLength = 0;
		*outHandles = NULL;
		return;
	}

	*outLength = 0;
	*outHandles = (Handle*)MemAlloc(doorCount * sizeof(**outHandles));

	// CRASH: Allocation failure
	if (!*outHandles) {
		TraceLog(LOG_ERROR, TextFormat("DOOR: Failed to allocate cache (size: %d)", doorCount));
		return;
	}

	for (j = 0, i = 0; i < doorCapacity; i++) {
		if (doorStatus[i].reserved) {
			(*outHandles)[j].index = i;
			(*outHandles)[j].version = doorStatus[i].version;
			j++;

			// BREAK: Leave early if we've found all the instances
			if (j == doorCount)
				break;
		}
	}

	*outLength = j;
}