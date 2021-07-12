#pragma once
#if ASCII_DEFINE
typedef enum ContainerType {
	CONTAINER_TYPE_CARCASS,
	CONTAINER_TYPE_CHEST,
	CONTAINER_TYPE_LOOT
} ContainerType;

#include "Handle.h"

void InitContainers(int capacity);
int MaxContainers(void);

bool IsContainerValid(Handle container);
Handle SpawnContainer(ContainerType containerType); // Requires a sprite and inventory to be available
void DestroyContainer(Handle container);
void DestroyAllContainers(void);
int CountContainers(void);

Handle GetContainerAtPosition(Vector2 position);
Handle GetContainerInventory(Handle container);
Vector2 GetContainerPosition(Handle container);
ContainerType GetContainerType(Handle container);
void SetContainerPosition(Handle container, Vector2 position);

TerminalTile GetCarcassContainerButcheredTile(Handle carcassContainer);
int GetCarcassContainerDecayDuration(Handle carcassContainer);
int GetCarcassContainerDecayTime(Handle carcassContainer);
TerminalTile GetCarcassContainerTile(Handle carcassContainer);
bool IsCarcassContainerButchered(Handle carcassContainer);
void ResetCarcassContainerDecayTime(Handle carcassContainer);
void SetCarcassContainerButchered(Handle carcassContainer, bool butchered);
void SetCarcassContainerButcheredTile(Handle carcassContainer, TerminalTile butcheredTile);
void SetCarcassContainerDecayDuration(Handle carcassContainer, int decayDuration);
void SetCarcassContainerTile(Handle carcassContainer, TerminalTile tile);

TerminalTile GetChestContainerClosedTile(Handle chestContainer);
TerminalTile GetChestContainerOpenedTile(Handle chestContainer);
bool IsChestContainerOpened(Handle chestContainer);
void SetChestContainerClosedTile(Handle chestContainer, TerminalTile closedTile);
void SetChestContainerOpenedTile(Handle chestContainer, TerminalTile openedTile);
void SetChestContainerOpened(Handle chestContainer, bool opened);

int GetLootContainerDecayDuration(Handle lootContainer);
int GetLootContainerDecayTime(Handle lootContainer);
TerminalTile GetLootContainerTile(Handle lootContainer);
void ResetLootContainerDecayTime(Handle lootContainer);
void SetLootContainerDecayDuration(Handle lootContainer, int decayDuration);
void SetLootContainerTile(Handle lootContainer, TerminalTile tile);

void RemoveDecayedContainers(void);
#endif