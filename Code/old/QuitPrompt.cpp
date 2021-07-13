#include "stdafx.h"
#if ASCII_DEFINE
#include "Prompt.h"

void QuitApplication(void);

void OpenQuitPrompt(void)
{
	OpenBoolPrompt(QuitApplication, NULL, "Exit to Desktop", "Are you sure you want to quit?", "Yes", "No");
}
#endif