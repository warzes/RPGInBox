#pragma once

typedef void (*BoolPromptCallback)(void);

#define PROMPT_COLOR CLITERAL(Color){ 0, 0, 0, 128 }
#define PROMPT_TILE CLITERAL(TerminalTile) { .background = PROMPT_COLOR, .foreground = PROMPT_COLOR, .symbol = ' ' }

void OpenBoolPrompt(BoolPromptCallback onAccept, BoolPromptCallback onDecline, const char* titleText, const char* messageText, const char* acceptText, const char* declineText);
void OpenMessagePrompt(const char* titleText, const char* messageText, const char* acceptText);
void OpenStringPrompt(char** outString, int maxStringLength, const char* titleText, const char* messageText, const char* acceptText);

void OpenLevelUpPrompt(int level);      // Message prompt that handles level messaging/options
void OpenNewGamePrompt(void);           // Bool prompt for starting a new game while a game is in play
void OpenQuitGamePrompt(void);          // Bool prompt for quitting a game while a game is in play
void OpenQuitPrompt(void);              // Bool prompt for quitting the application
