#include "stdafx.h"
#include "Game.h"
#include "Creature.h"
#include "Dungeon.h"
#include "Handle.h"
#include "Sprite.h"
#include "View.h"

void NewGame(void)
{
	// Cleanup
	DestroyAllCreatures();

	// Setup
	SpawnProtagonist();
	GenerateBasicDungeon();
}

void LoadGame(const char* fileName)
{
	// TODO
}

void SaveGame(const char* fileName)
{
	// TODO
}

void GameOver(void)
{
	// TODO: Temporary
	SetView(&VIEW_MAIN_MENU);
}

Handle SpawnProtagonist(void)
{
	static const TerminalTile playerTile = { .background = ALPHA_BLACK, .foreground = WHITE, .symbol = '@' };
	Handle protagonist;

	// Destroy the previous protagonist
	protagonist = GetCreatureProtagonist();

	if (IsCreatureValid(protagonist))
		DestroyCreature(protagonist);

	protagonist = SpawnCreature();

	// CRASH: Protagonist failed to spawn
	if (!IsCreatureValid(protagonist)) {
		TraceLog(LOG_ERROR, "GAME: Failed to spawn the protagonist");
		return NULL_HANDLE;
	}

	// Initialize protagonist
	SetCreatureProtagonist(protagonist);
	SetCreatureStat(protagonist, CREATURE_STAT_LEVEL, 1);
	SetSpriteTile(GetCreatureSprite(protagonist), playerTile);

	return protagonist;
}