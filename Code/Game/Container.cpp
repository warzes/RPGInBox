#include "stdafx.h"
#if ASCII_DEFINE
#include "Container.h"
#include "Inventory.h"
#include "Sprite.h"

static void CacheActiveContainers(int* outLength, Handle* outHandles[]);

static int containerCapacity;
static int containerCount;
static int containerLowestFree;
static struct ContainerStatus {
	bool reserved;
	unsigned int version;
} *containerStatus;
static struct ContainerData {
	Vector2 position;
	Handle sprite;
	Handle inventory;
	ContainerType type;
	union {
		struct {
			bool butchered;
			int decayTime;
			int decayDuration;
			TerminalTile butcheredTile;
			TerminalTile tile;
		} carcass;
		struct {
			bool opened;
			TerminalTile closedTile;
			TerminalTile openedTile;
		} chest;
		struct {
			int decayTime;
			int decayDuration;
			TerminalTile tile;
		} loot;
	} value;
} *containerData;

void InitContainers(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	containerStatus = (ContainerStatus*)MemAlloc(capacity * sizeof(*containerStatus));
	containerData = (ContainerData*)MemAlloc(capacity * sizeof(*containerData));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!containerStatus || !containerData) {
		TraceLog(LOG_ERROR, TextFormat("CONTAINER: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	containerCapacity = capacity;

	for (i = 0; i < containerCapacity; i++) {
		containerStatus[i].reserved = false;
		containerStatus[i].version = 0;
	}

	for (i = 0; i < containerCapacity; i++) {
		containerData[i].sprite = NULL_HANDLE;
		containerData[i].inventory = NULL_HANDLE;
	}

	containerCount = 0;
	containerLowestFree = 0;
	TraceLog(LOG_INFO, TextFormat("CONTAINER: Initialized successfully (capacity: %d)", containerCapacity));
}

inline int MaxContainers(void)
{
	return containerCapacity;
}

bool IsContainerValid(Handle container)
{
	return !IsHandleNull(container)
		&& containerStatus[container.index].reserved
		&& containerStatus[container.index].version == container.version;
}

Handle SpawnContainer(ContainerType containerType)
{
	int i;

	for (i = containerLowestFree; i < containerCapacity; i++) {
		if (!containerStatus[i].reserved) {
			int j;
			Handle handle;

			// Update instance status
			containerStatus[i].reserved = true;
			containerStatus[i].version++;

			// Make handle
			handle.version = containerStatus[i].version;
			handle.index = i;

			// Update module status
			containerCount++;
			containerLowestFree = i + 1;

			// Prepare instance
			containerData[i].type = containerType;
			containerData[i].position.x = 0;
			containerData[i].position.y = 0;
			containerData[i].sprite = SpawnSprite();
			containerData[i].inventory = SpawnInventory();

			// EXIT: Sprite required
			if (!IsSpriteValid(containerData[i].sprite)) {
				TraceLog(LOG_WARNING, TextFormat("CONTAINER: Failed to spawn because there are not enough sprites (%d/%d)", CountSprites(), MaxSprites()));
				DestroyContainer(handle);
				return NULL_HANDLE;
			}

			// EXIT: Inventory required
			if (!IsInventoryValid(containerData[i].inventory)) {
				TraceLog(LOG_WARNING, TextFormat("CONTAINER: Failed to spawn because there are not enough inventories (%d/%d)", CountInventories(), MaxInventories()));
				DestroyContainer(handle);
				return NULL_HANDLE;
			}

			// Prepare instance (continued)
			switch (containerType) {
			case CONTAINER_TYPE_CARCASS:
				containerData[i].value.carcass.butchered = false;
				containerData[i].value.carcass.decayTime = 30;
				containerData[i].value.carcass.decayDuration = 30;
				containerData[i].value.carcass.butcheredTile = TerminalTile{ .background = ALPHA_BLACK, .foreground = RED, .symbol = '&' };
				containerData[i].value.carcass.tile = TerminalTile{ .background = ALPHA_BLACK, .foreground = WHITE, .symbol = '%' };
				SetSpriteTile(containerData[i].sprite, containerData[i].value.carcass.tile);
				break;
			case CONTAINER_TYPE_CHEST:
				containerData[i].value.chest.opened = false;
				containerData[i].value.chest.closedTile = TerminalTile{ .background = ALPHA_BLACK, .foreground = BROWN, .symbol = '=' };
				containerData[i].value.chest.openedTile = TerminalTile{ .background = ALPHA_BLACK, .foreground = BROWN, .symbol = '-' };
				SetSpriteTile(containerData[i].sprite, containerData[i].value.chest.closedTile);
				break;
			case CONTAINER_TYPE_LOOT:
				containerData[i].value.loot.decayTime = 0;
				containerData[i].value.loot.decayDuration = 0;
				containerData[i].value.loot.tile = TerminalTile{ .background = ALPHA_BLACK, .foreground = GOLD, .symbol = '$' };
				SetSpriteTile(containerData[i].sprite, containerData[i].value.loot.tile);
				break;
			default:
				// CRASH: Implementation failure
				TraceLog(LOG_ERROR, TextFormat("CONTAINER: SpawnContainer() case %d is not implemented", containerType));
				return NULL_HANDLE;
			}

			SetSpriteLayer(containerData[i].sprite, SPRITE_LAYER_CONTAINER);

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroyContainer(Handle container)
{
	containerStatus[container.index].reserved = false;

	if (IsSpriteValid(containerData[container.index].sprite)) {
		DestroySprite(containerData[container.index].sprite);
		containerData[container.index].sprite = NULL_HANDLE;
	}

	if (IsInventoryValid(containerData[container.index].sprite)) {
		DestroyInventory(containerData[container.index].inventory);
		containerData[container.index].inventory = NULL_HANDLE;
	}

	containerCount--;

	if (container.index < containerLowestFree)
		containerLowestFree = container.index;
}

void DestroyAllContainers(void)
{
	int i;

	for (i = 0; i < containerCapacity; i++) {
		containerStatus[i].reserved = false;

		if (IsSpriteValid(containerData[i].sprite)) {
			DestroySprite(containerData[i].sprite);
			containerData[i].sprite = NULL_HANDLE;
		}

		if (IsInventoryValid(containerData[i].sprite)) {
			DestroyInventory(containerData[i].inventory);
			containerData[i].inventory = NULL_HANDLE;
		}
	}

	containerCount = 0;
	containerLowestFree = 0;
}

inline int CountContainers(void)
{
	return containerCount;
}

Handle GetContainerAtPosition(Vector2 position)
{
	int i;
	int length;
	Handle found;
	Handle* cache;

	CacheActiveContainers(&length, &cache);

	if (!cache)
		return NULL_HANDLE;

	found = NULL_HANDLE;

	for (i = 0; i < length; i++) {
		if (containerData[cache[i].index].position.x == position.x
			&& containerData[cache[i].index].position.y == position.y) {
			found = cache[i];
			break;
		}
	}

	MemFree(cache);
	return found;
}

Handle GetContainerInventory(Handle container)
{
	return containerData[container.index].inventory;
}

Vector2 GetContainerPosition(Handle container)
{
	return containerData[container.index].position;
}

ContainerType GetContainerType(Handle container)
{
	return containerData[container.index].type;
}

void SetContainerPosition(Handle container, Vector2 position)
{
	containerData[container.index].position = position;
	SetSpritePosition(containerData[container.index].sprite, position);
}

TerminalTile GetCarcassContainerButcheredTile(Handle carcassContainer)
{
	return containerData[carcassContainer.index].value.carcass.butcheredTile;
}

int GetCarcassContainerDecayDuration(Handle carcassContainer)
{
	return containerData[carcassContainer.index].value.carcass.decayDuration;
}

int GetCarcassContainerDecayTime(Handle carcassContainer)
{
	return containerData[carcassContainer.index].value.carcass.decayTime;
}

TerminalTile GetCarcassContainerTile(Handle carcassContainer)
{
	return containerData[carcassContainer.index].value.carcass.tile;
}

bool IsCarcassContainerButchered(Handle carcassContainer)
{
	return containerData[carcassContainer.index].value.carcass.butchered;
}

void ResetCarcassContainerDecayTime(Handle carcassContainer)
{
	containerData[carcassContainer.index].value.carcass.decayTime = containerData[carcassContainer.index].value.carcass.decayDuration;
}

void SetCarcassContainerButchered(Handle carcassContainer, bool butchered)
{
	containerData[carcassContainer.index].value.carcass.butchered = butchered;
}

void SetCarcassContainerButcheredTile(Handle carcassContainer, TerminalTile butcheredTile)
{
	containerData[carcassContainer.index].value.carcass.butcheredTile = butcheredTile;

	if (IsCarcassContainerButchered(carcassContainer))
		SetSpriteTile(containerData[carcassContainer.index].sprite, butcheredTile);
}

void SetCarcassContainerDecayDuration(Handle carcassContainer, int decayDuration)
{
	containerData[carcassContainer.index].value.carcass.decayDuration = decayDuration;
}

void SetCarcassContainerTile(Handle carcassContainer, TerminalTile tile)
{
	containerData[carcassContainer.index].value.carcass.tile = tile;

	if (!IsCarcassContainerButchered(carcassContainer))
		SetSpriteTile(containerData[carcassContainer.index].sprite, tile);
}

TerminalTile GetChestContainerClosedTile(Handle chestContainer)
{
	return containerData[chestContainer.index].value.chest.closedTile;
}

TerminalTile GetChestContainerOpenedTile(Handle chestContainer)
{
	return containerData[chestContainer.index].value.chest.openedTile;
}

bool IsChestContainerOpened(Handle chestContainer)
{
	return containerData[chestContainer.index].value.chest.opened;
}

void SetChestContainerClosedTile(Handle chestContainer, TerminalTile closedTile)
{
	containerData[chestContainer.index].value.chest.closedTile = closedTile;

	if (!IsChestContainerOpened(chestContainer))
		SetSpriteTile(containerData[chestContainer.index].sprite, closedTile);
}

void SetChestContainerOpenedTile(Handle chestContainer, TerminalTile openedTile)
{
	containerData[chestContainer.index].value.chest.openedTile = openedTile;

	if (IsChestContainerOpened(chestContainer))
		SetSpriteTile(containerData[chestContainer.index].sprite, openedTile);
}

void SetChestContainerOpened(Handle chestContainer, bool opened)
{
	if (IsChestContainerOpened(chestContainer)) {
		if (!opened) {
			containerData[chestContainer.index].value.chest.opened = false;
			SetSpriteTile(
				containerData[chestContainer.index].sprite,
				containerData[chestContainer.index].value.chest.closedTile);
		}
	}
	else {
		if (opened) {
			containerData[chestContainer.index].value.chest.opened = true;
			SetSpriteTile(
				containerData[chestContainer.index].sprite,
				containerData[chestContainer.index].value.chest.openedTile);
		}
	}
}

int GetLootContainerDecayDuration(Handle lootContainer)
{
	return containerData[lootContainer.index].value.loot.decayDuration;
}

int GetLootContainerDecayTime(Handle lootContainer)
{
	return containerData[lootContainer.index].value.loot.decayTime;
}

TerminalTile GetLootContainerTile(Handle lootContainer)
{
	return containerData[lootContainer.index].value.loot.tile;
}

void ResetLootContainerDecayTime(Handle lootContainer)
{
	containerData[lootContainer.index].value.loot.decayTime = containerData[lootContainer.index].value.loot.decayDuration;
}

void SetLootContainerDecayDuration(Handle lootContainer, int decayDuration)
{
	containerData[lootContainer.index].value.loot.decayDuration = decayDuration;
}

void SetLootContainerTile(Handle lootContainer, TerminalTile tile)
{
	containerData[lootContainer.index].value.loot.tile = tile;
	SetSpriteTile(containerData[lootContainer.index].sprite, tile);
}

void RemoveDecayedContainers(void)
{
	// TODO: Cache active
}

static void CacheActiveContainers(int* outLength, Handle* outHandles[])
{
	int i;
	int j;

	// EXIT: There are no containers
	if (containerCount == 0) {
		*outLength = 0;
		*outHandles = NULL;
		return;
	}

	*outLength = 0;
	*outHandles = (Handle*)MemAlloc(containerCount * sizeof(**outHandles));

	// CRASH: Allocation failure
	if (!*outHandles) {
		TraceLog(LOG_ERROR, TextFormat("CONTAINER: Failed to allocate cache (size: %d)", containerCount));
		return;
	}

	for (j = 0, i = 0; i < containerCapacity; i++) 
	{
		if (containerStatus[i].reserved)
		{
			(*outHandles)[j].index = i;
			(*outHandles)[j].version = containerStatus[i].version;
			j++;

			// BREAK: Leave early if we've found all the instances
			if (j == containerCount)
				break;
		}
	}

	*outLength = j;
}
#endif