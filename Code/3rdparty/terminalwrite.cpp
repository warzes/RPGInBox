#include "terminalwrite.h"

#include "Raylib/raymath.h"
#include <stdint.h>

static int terminalCursorX;
static int terminalCursorY;
static bool terminalWrapHorizontal = true;
static bool terminalWrapVertical = true;
static Color terminalBackPaint = { 0, 0, 0, 255 };// BLACK;
static Color terminalForePaint = { 80, 80, 80, 255 };// DARKGRAY;

//----------------------------------------------------------------------------------
// Terminal cursor functions
//----------------------------------------------------------------------------------
bool GetTerminalCursorWrapHorizontal(void)
{
	return terminalWrapHorizontal;
}

bool GetTerminalCursorWrapVertical(void)
{
	return terminalWrapVertical;
}

int GetTerminalCursorX(void)
{
	return terminalCursorX;
}

int GetTerminalCursorY(void)
{
	return terminalCursorY;
}

bool MoveTerminalCursorNext(void)
{
	return MoveTerminalCursorRight(1);
}

bool MoveTerminalCursorBack(void)
{
	return MoveTerminalCursorLeft(1);
}

bool MoveTerminalCursorNextLine(void)
{
	SnapTerminalCursorLeft();
	return MoveTerminalCursorDown(1);
}

bool MoveTerminalCursorPreviousLine(void)
{
	SnapTerminalCursorLeft();
	return MoveTerminalCursorUp(1);
}

bool MoveTerminalCursorUp(int distance)
{
	int i;

	for (i = 0; i < distance; i++) {
		terminalCursorY--;

		if (terminalCursorY < 0) {
			terminalCursorY = (terminalWrapVertical) ? GetTerminalHeight() - 1 : 0;
			return false;
		}
	}

	return true;
}

bool MoveTerminalCursorRight(int distance)
{
	int i;

	for (i = 0; i < distance; i++) {
		terminalCursorX++;

		if (terminalCursorX == GetTerminalWidth()) {
			terminalCursorX = (terminalWrapHorizontal) ? 0 : GetTerminalWidth() - 1;
			return false;
		}
	}

	return true;
}

bool MoveTerminalCursorDown(int distance)
{
	int i;

	for (i = 0; i < distance; i++) {
		terminalCursorY++;

		if (terminalCursorY == GetTerminalHeight()) {
			terminalCursorY = (terminalWrapVertical) ? 0 : GetTerminalHeight() - 1;
			return false;
		}
	}

	return true;
}

bool MoveTerminalCursorLeft(int distance)
{
	int i;

	for (i = 0; i < distance; i++) {
		terminalCursorX--;

		if (terminalCursorX < 0) {
			terminalCursorX = (terminalWrapHorizontal) ? GetTerminalWidth() - 1 : 0;
			return false;
		}
	}

	return true;
}

void SnapTerminalCursorBegin(void)
{
	terminalCursorX = 0;
	terminalCursorY = 0;
}

void SnapTerminalCursorEnd(void)
{
	terminalCursorX = GetTerminalWidth() - 1;
	terminalCursorY = GetTerminalHeight() - 1;
}

void SnapTerminalCursorLineBegin(void)
{
	terminalCursorX = 0;
}

void SnapTerminalCursorLineEnd(void)
{
	terminalCursorX = GetTerminalHeight() - 1;
}

void SnapTerminalCursorUp(void)
{
	terminalCursorY = 0;
}

void SnapTerminalCursorRight(void)
{
	terminalCursorX = GetTerminalWidth() - 1;
}

void SnapTerminalCursorDown(void)
{
	terminalCursorY = GetTerminalHeight() - 1;
}

void SnapTerminalCursorLeft(void)
{
	terminalCursorX = 0;
}

void SetTerminalCursorWrap(bool wrapHorizontal, bool wrapVertical)
{
	terminalWrapHorizontal = wrapHorizontal;
	terminalWrapVertical = wrapVertical;
}

inline void SetTerminalCursorXY(int x, int y)
{
	x = Clamp(x, 0, GetTerminalWidth() - 1);
	y = Clamp(y, 0, GetTerminalHeight() - 1);
	terminalCursorX = x;
	terminalCursorY = y;
}

//----------------------------------------------------------------------------------
// Tile writing functions
//----------------------------------------------------------------------------------
Color GetTerminalWriteBackPaint(void)
{
	return terminalBackPaint;
}

Color GetTerminalWriteForePaint(void)
{
	return terminalForePaint;
}

void SetTerminalWriteBackPaint(Color backPaint)
{
	terminalBackPaint = backPaint;
}

void SetTerminalWriteForePaint(Color forePaint)
{
	terminalForePaint = forePaint;
}

void WriteTerminalLetter(char letter)
{
	TerminalTile tile;

	if (letter == '\0') {
		return;
	} else if (letter == '\n' || letter == '\r') {
		MoveTerminalCursorNextLine();
	} else {
		tile.background = ColorAlphaBlend(GetTerminalTile(terminalCursorX, terminalCursorY).background, terminalBackPaint, WHITE);
		tile.foreground = ColorAlphaBlend(GetTerminalTile(terminalCursorX, terminalCursorY).foreground, terminalForePaint, WHITE);
		tile.symbol = letter;
		SetTerminalTile(terminalCursorX, terminalCursorY, tile);
		MoveTerminalCursorNext();
	}
}

void WriteTerminalText(const char *text)
{
	return WriteTerminalTextLength(text, INT32_MAX);
}

void WriteTerminalTextLength(const char *text, int maxLength)
{
	int i;

	i = 0;

	// Always move the cursor before breaking the loop
	do {
		TerminalTile tile;

		tile.background = ColorAlphaBlend(GetTerminalTile(terminalCursorX, terminalCursorY).background, terminalBackPaint, WHITE);
		tile.foreground = ColorAlphaBlend(GetTerminalTile(terminalCursorX, terminalCursorY).foreground, terminalForePaint, WHITE);

		if (text[i] == '\n' || text[i] == '\r') {
			// BREAK: The cursor hit an edge
			if (MoveTerminalCursorNextLine())
				break;
		} else {
			tile.symbol = (text[i] == '\0') ? ' ' : text[i];
			SetTerminalTile(terminalCursorX, terminalCursorY, tile);
		}

		i++;
	} while (MoveTerminalCursorNext() && i < maxLength && text[i] != '\0');
}

//----------------------------------------------------------------------------------
// Terminal write save/load state
//----------------------------------------------------------------------------------
TerminalWriteSettings StoreTerminalWriteSettings(void)
{
	TerminalWriteSettings result;

	result.backPaint = GetTerminalWriteBackPaint();
	result.forePaint = GetTerminalWriteForePaint();
	result.wrapHorizontal = GetTerminalCursorWrapHorizontal();
	result.wrapVertical = GetTerminalCursorWrapVertical();
	result.cursorX = GetTerminalCursorX();
	result.cursorY = GetTerminalCursorY();

	return result;
}

void RestoreTerminalWriteSettings(TerminalWriteSettings settings)
{
	SetTerminalWriteBackPaint(settings.backPaint);
	SetTerminalWriteForePaint(settings.forePaint);
	SetTerminalCursorWrap(settings.wrapHorizontal, settings.wrapVertical);
	SetTerminalCursorXY(settings.cursorX, settings.cursorY);
}
