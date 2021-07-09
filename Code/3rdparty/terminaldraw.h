#ifndef TERMINAL_DRAW_H
#define TERMINAL_DRAW_H

#include "raylib/raylib.h"
#include "terminal.h"

void DrawTerminalBox(int x, int y, int width, int height, TerminalTile fill, TerminalTile outline);
void DrawTerminalBoxFill(int x, int y, int width, int height, TerminalTile fill);
void DrawTerminalBoxOutline(int x, int y, int width, int height, TerminalTile outline);

#endif // TERMINAL_DRAW_H
