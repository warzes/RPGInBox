#pragma once
#if ASCII_DEFINE
typedef enum ItemType {
	ITEM_TYPE_BOOK,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_MISC
} ItemType;

typedef enum ItemPrefab {
	ITEM_PREFAB_NONE,
	// Books
	ITEM_PREFAB_BOOK_VAMPIRE_SCROLL,
	// Weapons
	ITEM_PREFAB_WEAP_IRON_LONGSWORD,
	// Size
	ITEM_PREFAB__LENGTH
} ItemPrefab;

#define ITEM_NAME_LENGTH 32
#define ITEM_DESCRIPTION_LENGTH 128

void InitItemPrefabs(void);
int CountItems(void);

const char* GetItemDescription(ItemPrefab item); // Owned return
const char* GetItemName(ItemPrefab item); // Owned return
int GetItemPrice(ItemPrefab item);
ItemType GetItemType(ItemPrefab item);
float GetItemWeight(ItemPrefab item);

#endif