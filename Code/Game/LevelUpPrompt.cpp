#include "stdafx.h"
#if ASCII_DEFINE
#include "Prompt.h"
#include "Game.h"
#include "View.h"

void OpenLevelUpPrompt(int level)
{
	OpenMessagePrompt("Level Up", TextFormat("Congratulations, you are now level %d.", level), "Onward!");
}
#endif
