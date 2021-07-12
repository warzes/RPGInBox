#include "stdafx.h"
#include "Item.h"

typedef struct ItemPrefabInfo {
	ItemType type;
	const char* name;
	const char* description;
	int price;
	float weight;
} ItemPrefabInfo;

static struct ItemPrefabInfo itemPrefabData[ITEM_PREFAB__LENGTH];

void InitItemPrefabs(void)
{
	// Books
	itemPrefabData[ITEM_PREFAB_BOOK_VAMPIRE_SCROLL] = ItemPrefabInfo{
		.type = ITEM_TYPE_BOOK,//ITEM_PREFAB_BOOK_VAMPIRE_SCROLL,
		.name = "The Vampire Scroll",
		.description = "A scroll made of an unknown material with pearlescent text. Ancient, yet pristine.",
		.price = 100000000,
		.weight = 0.01
	};

	// Weapons
	itemPrefabData[ITEM_PREFAB_WEAP_IRON_LONGSWORD] = ItemPrefabInfo{
		.type = ITEM_TYPE_WEAPON,
		.name = "Iron Longsword",
		.description = "A military-issue longsword made of iron.",
		.price = 10,
		.weight = 1.5
	};
}

int CountItems(void)
{
	return ITEM_PREFAB__LENGTH;
}

const char* GetItemDescription(ItemPrefab item)
{
	return itemPrefabData[item].description;
}

const char* GetItemName(ItemPrefab item)
{
	return itemPrefabData[item].name;
}

int GetItemPrice(ItemPrefab item)
{
	return itemPrefabData[item].price;
}

ItemType GetItemType(ItemPrefab item)
{
	return itemPrefabData[item].type;
}

float GetItemWeight(ItemPrefab item)
{
	return itemPrefabData[item].weight;
}