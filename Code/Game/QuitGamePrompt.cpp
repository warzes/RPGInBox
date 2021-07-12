#include "stdafx.h"
#include "Prompt.h"

#include "View.h"

static void OnAccept(void)
{
	SetView(&VIEW_MAIN_MENU);
}

void OpenQuitGamePrompt(void)
{
	OpenBoolPrompt(OnAccept, NULL, "Quit to Main Menu", "Quit the current game and go back to the main menu? Unsaved changes will be lost.", "Accept", "Decline");
}