#pragma once
#if ASCII_DEFINE
#include "Handle.h"

void NewGame(void);
void LoadGame(const char* fileName);
void SaveGame(const char* fileName);
void GameOver(void);
Handle SpawnProtagonist(void);
#endif