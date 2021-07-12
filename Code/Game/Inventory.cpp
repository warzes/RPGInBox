#include "stdafx.h"
#include "Inventory.h"

static int inventoryCapacity;
static int inventoryCount;
static int inventoryLowestFree;
static struct InventoryStatus {
	bool reserved;
	unsigned int version;
} *inventoryStatus;
static struct InventoryData {
	ItemPrefab items[MAX_INVENTORY_ITEM_QUANTITY];
} *inventoryData;

void InitInventories(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	inventoryStatus = (InventoryStatus*)MemAlloc(capacity * sizeof(*inventoryStatus));
	inventoryData = (InventoryData*)MemAlloc(capacity * sizeof(*inventoryData));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!inventoryStatus || !inventoryData) {
		TraceLog(LOG_ERROR, TextFormat("INVENTORY: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	inventoryCapacity = capacity;

	for (i = 0; i < inventoryCapacity; i++) {
		inventoryStatus[i].reserved = false;
		inventoryStatus[i].version = 0;
	}

	inventoryCount = 0;
	inventoryLowestFree = 0;
	TraceLog(LOG_INFO, TextFormat("INVENTORY: Initialized successfully (capacity: %d)", inventoryCapacity));
}

int MaxInventories(void)
{
	return inventoryCapacity;
}

bool IsInventoryValid(Handle inventory)
{
	return !IsHandleNull(inventory)
		&& inventoryStatus[inventory.index].reserved
		&& inventoryStatus[inventory.index].version == inventory.version;
}

Handle SpawnInventory(void)
{
	int i;

	for (i = inventoryLowestFree; i < inventoryCapacity; i++) {
		if (!inventoryStatus[i].reserved) {
			int j;
			Handle handle;

			// Update instance status
			inventoryStatus[i].reserved = true;
			inventoryStatus[i].version++;

			// Make handle
			handle.version = inventoryStatus[i].version;
			handle.index = i;

			// Update module status
			inventoryCount++;
			inventoryLowestFree = i + 1;

			// Prepare instance
			for (j = 0; j < MAX_INVENTORY_ITEM_QUANTITY; j++)
				inventoryData[i].items[j] = ITEM_PREFAB_NONE;

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroyInventory(Handle inventory)
{
	inventoryStatus[inventory.index].reserved = false;
	inventoryCount--;

	if (inventory.index < inventoryLowestFree)
		inventoryLowestFree = inventory.index;
}

void DestroyAllInventories(void)
{
	int i;

	for (i = 0; i < inventoryCapacity; i++)
		inventoryStatus[i].reserved = false;

	inventoryCount = 0;
	inventoryLowestFree = 0;
}

int CountInventories(void)
{
	return inventoryCount;
}

int AddInventoryItem(Handle inventory, ItemPrefab itemPrefab, int quantity)
{
	int i;
	int totalAdded;

	totalAdded = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		if (inventoryData[inventory.index].items[i] == ITEM_PREFAB_NONE) {
			inventoryData[inventory.index].items[i] = itemPrefab;
			totalAdded++;

			if (totalAdded == quantity)
				return quantity;
		}
	}

	return totalAdded;
}

int CountAllInventoryItems(Handle inventory)
{
	int i;
	int sum;

	sum = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++)
		sum += inventoryData[inventory.index].items[i] != ITEM_PREFAB_NONE;

	return sum;
}

int CountInventoryItem(Handle inventory, ItemPrefab itemPrefab)
{
	int i;
	int sum;

	sum = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++)
		sum += inventoryData[inventory.index].items[i] == itemPrefab;

	return sum;
}

ItemPrefab GetInventoryItemAtIndex(Handle inventory, int index)
{
	return inventoryData[inventory.index].items[index];
}

int GetTotalInventoryPrice(Handle inventory)
{
	int i;
	int price;

	price = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		if (inventoryData[inventory.index].items[i] != ITEM_PREFAB_NONE)
			price += GetItemPrice(inventoryData[inventory.index].items[i]);
	}

	return price;
}

float GetTotalInventoryWeight(Handle inventory)
{
	int i;
	float weight;

	weight = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		if (inventoryData[inventory.index].items[i] != ITEM_PREFAB_NONE)
			weight += GetItemWeight(inventoryData[inventory.index].items[i]);
	}

	return weight;
}

bool IsInventoryEmpty(Handle inventory)
{
	int i;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		if (inventoryData[inventory.index].items[i] != ITEM_PREFAB_NONE)
			return false;
	}

	return true;
}

bool IsInventoryFull(Handle inventory)
{
	return CountAllInventoryItems(inventory) == MAX_INVENTORY_ITEM_QUANTITY;
}

int RemoveInventoryItem(Handle inventory, ItemPrefab itemPrefab, int quantity)
{
	int i;
	int totalRemoved;

	totalRemoved = 0;

	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		if (inventoryData[inventory.index].items[i] == itemPrefab) {
			inventoryData[inventory.index].items[i] = ITEM_PREFAB_NONE;
			totalRemoved++;

			if (totalRemoved == quantity)
				return quantity;
		}
	}

	return totalRemoved;
}