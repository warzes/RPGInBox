#ifndef TERMINAL_H
#define TERMINAL_H

#include "Raylib/raylib.h"

#define ALPHA_BLACK CLITERAL(Color){ 0, 0, 0, 0 }

typedef struct TerminalTile {
	Color background;
	Color foreground;
	char symbol;
} TerminalTile;

void InitTerminal(int width, int height);
void CloseTerminal(void);
void LoadTerminalFont(const char *fileName, int scale);
void ClearTerminal(void);
int GetTerminalXYtoI(int posX, int posY);
int GetTerminalWidth(void);
int GetTerminalHeight(void);
int GetTerminalFontWidth(void);
int GetTerminalFontHeight(void);
int GetTerminalScreenWidth(void);
int GetTerminalScreenHeight(void);
int GetTerminalFontScale(void);
TerminalTile GetTerminalTile(int posX, int posY);
TerminalTile *GetTerminalTileBuffer(void);
bool IsWithinTerminal(int posX, int posY);
bool IsWithinTerminalV(Vector2 position);
void SetTerminalFontScale(int scale);
void SetTerminalSize(int width, int height);
void SetTerminalTile(int posX, int posY, TerminalTile tile);
void SetTerminalTileV(Vector2 position, TerminalTile tile);
void DrawTerminal(void);

#endif // TERMINAL_H
