#ifndef TERMINAL_GUI_H
#define TERMINAL_GUI_H

#include "Raylib/raylib.h"
#include "terminal.h"

void DrawTerminalGuiButton(int x, int y, const char *text, int maxTextLength, Color background, Color foreground);
void DrawTerminalGuiFrame(int x, int y, int width, int height, const char *title, TerminalTile fill, TerminalTile outline, TerminalTile titleTile);
int DrawTerminalGuiTextArea(int x, int y, int width, int height, const char *text, Color background, Color foreground); // Returns the index of the start of the next word, if the full text could not be displayed

#endif // TERMINAL_GUI_H
