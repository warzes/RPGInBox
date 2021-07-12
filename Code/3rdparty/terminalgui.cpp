#include "terminalgui.h"

#include "terminaldraw.h"
#include "terminalwrite.h"
#include <string.h>

void DrawTerminalGuiButton(int x, int y, const char *text, int maxTextLength, Color background, Color foreground)
{
	TerminalWriteSettings previousSettings;
	Color prevBackPaint;
	Color prevForePaint;
	int prevCursorX;
	int prevCursorY;
	int textLength;
	int width;

	// Store terminal settings
	previousSettings = StoreTerminalWriteSettings();

	// Calculate info
	textLength = (text) ? strlen(text) : 0;
	textLength = (textLength > maxTextLength) ? maxTextLength : textLength;
	width = x + textLength + 2;

	// Write
	SetTerminalWriteBackPaint(background);
	SetTerminalWriteForePaint(foreground);
	SetTerminalCursorXY(x, y);
	WriteTerminalLetter(' ');

	if (textLength > 0)
		WriteTerminalTextLength(text, textLength);

	WriteTerminalLetter(' ');

	// Restore terminal settings
	RestoreTerminalWriteSettings(previousSettings);
}

void DrawTerminalGuiFrame(int x, int y, int width, int height, const char *titleText, TerminalTile fillTile, TerminalTile outlineTile, TerminalTile titleTile)
{
	DrawTerminalBox(x, y, width, height, fillTile, outlineTile);

	if (titleText) {
		TerminalWriteSettings previousSettings;
		int maxTitleLength;
		int titleLength;
		int titlePosX;

		// Store terminal settings
		previousSettings = StoreTerminalWriteSettings();

		// Calculate info
		maxTitleLength = width - 4;
		titleLength = strnlen(titleText, maxTitleLength);
		titlePosX = x + ((width - titleLength) / 2);

		// Write
		SetTerminalCursorWrap(false, false);
		SetTerminalWriteBackPaint(titleTile.background);
		SetTerminalWriteForePaint(titleTile.foreground);
		SetTerminalCursorXY(titlePosX - 1, y);
		WriteTerminalLetter(' ');
		WriteTerminalText(titleText);
		WriteTerminalLetter(' ');

		// Restore terminal settings
		RestoreTerminalWriteSettings(previousSettings);
	}
}

// TODO: Break per word for better wrapping. Also implement proper return behaviour
int DrawTerminalGuiTextArea(int x, int y, int width, int height, const char *text, Color background, Color foreground)
{
	int messageLength;
	int messageWidth;

	messageLength = text ? strlen(text) : 0;

	if (messageLength > 0) {
		TerminalWriteSettings previousSettings;
		int messageLineStart;
		int messageLengthRemainder;

		// Store terminal settings
		previousSettings = StoreTerminalWriteSettings();

		// Write
		messageLineStart = 0;
		messageLengthRemainder = messageLength;
		SetTerminalCursorWrap(true, true);
		SetTerminalCursorXY(x, y);
		SetTerminalWriteBackPaint(background);
		SetTerminalWriteForePaint(foreground);

		while (messageLengthRemainder > 0) {
			int messageLineEnd;

			messageLineEnd = (width < messageLengthRemainder) ? width : messageLengthRemainder;
			SetTerminalCursorXY(x, GetTerminalCursorY());
			WriteTerminalTextLength(&text[messageLineStart], messageLineEnd);
			messageLineStart += messageLineEnd;
			messageLengthRemainder -= messageLineEnd;
			MoveTerminalCursorNextLine();
		}

		// Restore terminal settings
		RestoreTerminalWriteSettings(previousSettings);
	}

	return 0;
}
