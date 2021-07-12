#include "stdafx.h"
#include "Prompt.h"

#include "Game.h"
#include "View.h"

static void OnAccept(void)
{
	NewGame();
	SetView(&VIEW_GAME_DEFAULT);
}

void OpenNewGamePrompt(void)
{
	OpenBoolPrompt(OnAccept, NULL, "New Game", "Quit the current game to start a new game? Unsaved changes will be lost.", "Accept", "Decline");
}