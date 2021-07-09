#ifndef TERMINAL_WRITE_H
#define TERMINAL_WRITE_H

#include "terminal.h"

typedef struct TerminalWriteSettings {
	Color backPaint;
	Color forePaint;
	bool wrapHorizontal;
	bool wrapVertical;
	int cursorX;
	int cursorY;
} TerminalWriteSettings;

//----------------------------------------------------------------------------------
// Terminal cursor functions
//----------------------------------------------------------------------------------
bool GetTerminalCursorWrapHorizontal(void);
bool GetTerminalCursorWrapVertical(void);
int GetTerminalCursorX(void);
int GetTerminalCursorY(void);
bool MoveTerminalCursorNext(void);                                      // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorBack(void);                                      // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorNextLine(void);                                  // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorPreviousLine(void);                              // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorUp(int distance);                                // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorRight(int distance);                             // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorDown(int distance);                              // Returns true if the cursor moved without wrapping or snapping
bool MoveTerminalCursorLeft(int distance);                              // Returns true if the cursor moved without wrapping or snapping
void SnapTerminalCursorBegin(void);
void SnapTerminalCursorEnd(void);
void SnapTerminalCursorLineBegin(void);
void SnapTerminalCursorLineEnd(void);
void SnapTerminalCursorUp(void);
void SnapTerminalCursorRight(void);
void SnapTerminalCursorDown(void);
void SnapTerminalCursorLeft(void);
void SetTerminalCursorWrap(bool wrapHorizontal, bool wrapVertical);
void SetTerminalCursorXY(int x, int y);

//----------------------------------------------------------------------------------
// Tile writing functions
//----------------------------------------------------------------------------------
Color GetTerminalWriteBackPaint(void);
Color GetTerminalWriteForePaint(void);
void SetTerminalWriteBackPaint(Color backPaint);
void SetTerminalWriteForePaint(Color forePaint);
void WriteTerminalLetter(char letter);
void WriteTerminalText(const char *text);
void WriteTerminalTextLength(const char *text, int maxLength);

//----------------------------------------------------------------------------------
// Terminal write save/load state
//----------------------------------------------------------------------------------
TerminalWriteSettings StoreTerminalWriteSettings(void);
void RestoreTerminalWriteSettings(TerminalWriteSettings settings);

#endif // TERMINAL_WRITE_H
