#include "stdafx.h"
#if ASCII_DEFINE
#include "Game/Prompt.h"

#include "Game/Input.h"
#include "Game/View.h"
#include "terminaldraw.h"
#include "terminalgui.h"
#include "terminalwrite.h"

typedef enum {
	BOOL_PROMPT_CURSOR_UNSELECTED,
	BOOL_PROMPT_CURSOR_ACCEPT,
	BOOL_PROMPT_CURSOR_DECLINE
} BoolPromptCursor;

#define BOOL_PROMPT_DEFAULT_ACCEPT "Accept"
#define BOOL_PROMPT_DEFAULT_DECLINE "Decline"

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

static const View BOOL_PROMPT_VIEW = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = false
};

static bool promptIsOpen;
static BoolPromptCallback promptOnAccept;
static BoolPromptCallback promptOnDecline;
static const char* promptTitleText;
static const char* promptMessageText;
static const char* promptAcceptText;
static const char* promptDeclineText;
static BoolPromptCursor promptSelection;

void OpenBoolPrompt(BoolPromptCallback onAccept, BoolPromptCallback onDecline, const char* titleText, const char* messageText, const char* acceptText, const char* declineText)
{
	// CRASH: Only one prompt allowed at a time
	if (promptIsOpen) {
		TraceLog(LOG_ERROR, "PROMPT: BoolPrompt is already open");
		return;
	}

	promptIsOpen = true;
	promptOnAccept = onAccept;
	promptOnDecline = onDecline;
	promptTitleText = titleText;
	promptMessageText = messageText;
	promptAcceptText = (acceptText) ? acceptText : BOOL_PROMPT_DEFAULT_ACCEPT;
	promptDeclineText = (declineText) ? declineText : BOOL_PROMPT_DEFAULT_DECLINE;

	PushView(&BOOL_PROMPT_VIEW);
}

static void OnOpenView(void)
{
	promptSelection = BOOL_PROMPT_CURSOR_UNSELECTED;
}

static void OnCloseView(void)
{
	promptIsOpen = false;
}

static void OnControlView(void)
{
	if (IsInputActive(INPUT_UI_CANCEL)) {
		PopView();

		if (promptOnDecline)
			promptOnDecline();
	}
	else if (IsInputActive(INPUT_UI_SUBMIT)) {
		if (promptSelection == BOOL_PROMPT_CURSOR_ACCEPT) {
			PopView();

			if (promptOnAccept)
				promptOnAccept();
		}
		else if (promptSelection == BOOL_PROMPT_CURSOR_DECLINE) {
			PopView();

			if (promptOnDecline)
				promptOnDecline();
		}
	}
	else if (IsInputActive(INPUT_UI_LEFT)) {
		if (promptAcceptText)
			promptSelection = BOOL_PROMPT_CURSOR_ACCEPT;
	}
	else if (IsInputActive(INPUT_UI_RIGHT)) {
		if (promptDeclineText)
			promptSelection = BOOL_PROMPT_CURSOR_DECLINE;
	}
	else if (IsInputActive(INPUT_UI_UP)) {
		promptSelection = BOOL_PROMPT_CURSOR_UNSELECTED;
	}
}

static void OnRenderView(void)
{
	static const int frameWidth = 40;
	static const TerminalTile frameFill = { .background = BLACK, .foreground = WHITE, .symbol = ' ' };
	static const TerminalTile frameOutline = { .foreground = DARKGRAY, .symbol = (char)203 };
	static const TerminalTile frameOutlineTitle = { .background = GRAY, .foreground = BLACK };
	int messageLength;
	int messageWidth;
	int messageHeight;
	int framePosX;
	int framePosY;
	int frameHeight;

	// Fade entire background
	// Box fill does not support transparency, so this doesn't behave as expected
//	DrawTerminalBoxFill(0, 0, GetTerminalWidth(), GetTerminalHeight(), PROMPT_TILE);

	// Calculate message info
	messageLength = promptMessageText ? strlen(promptMessageText) : 0;
	messageWidth = frameWidth - 4;
	messageHeight = (messageLength > 0) ? messageLength / messageWidth : 0;

	// Layout frame
	frameHeight = 7 + messageHeight;
	framePosX = GetTerminalWidth() - frameWidth;
	framePosX = (framePosX > 0) ? framePosX / 2 : 0;
	framePosY = GetTerminalHeight() - frameHeight;
	framePosY = (framePosY > 0) ? framePosY / 2 : 0;
	DrawTerminalGuiFrame(framePosX, framePosY, frameWidth, frameHeight, promptTitleText, frameFill, frameOutline, frameOutlineTitle);

	// Layout message
	DrawTerminalGuiTextArea(framePosX + 2, framePosY + 2, frameWidth - 4, frameHeight - 5, promptMessageText, frameFill.background, frameFill.foreground);
	/*
	if (messageLength > 0) {
		int messageLineStart;
		int messageLengthRemainder;
		int messagePosX;
		int messagePosY;

		messageLineStart = 0;
		messageLengthRemainder = messageLength;
		messagePosX = framePosX + 2;
		messagePosY = framePosY + 2;
		SetTerminalCursorWrap(true, true);
		SetTerminalCursorXY(messagePosX, messagePosY);
		SetTerminalWriteBackPaint(frameFill.background);
		SetTerminalWriteForePaint(frameFill.foreground);

		while (messageLengthRemainder > 0) {
			int messageLineEnd;

			messageLineEnd = (messageWidth < messageLengthRemainder) ? messageWidth : messageLengthRemainder;
			SetTerminalCursorXY(messagePosX, GetTerminalCursorY());
			WriteTerminalTextLength(&promptMessageText[messageLineStart], messageLineEnd);
			messageLineStart += messageLineEnd;
			messageLengthRemainder -= messageLineEnd;
			MoveTerminalCursorNextLine();
		}
	}
	*/

	// Layout options
	{
		int optionPosY;
		int acceptLength;
		int declineLength;
		int acceptStart;
		int declineStart;
		int totalLength;
		Color acceptForeground;
		Color acceptBackground;
		Color declineForeground;
		Color declineBackground;

		optionPosY = framePosY + frameHeight - 3;
		acceptLength = strlen(promptAcceptText);
		declineLength = strlen(promptDeclineText);
		totalLength = (acceptLength + 2) + 2 + (declineLength + 2);
		acceptStart = framePosX + ((frameWidth - totalLength) / 2);
		declineStart = (acceptStart + acceptLength) + 3;
		acceptForeground = (promptSelection == BOOL_PROMPT_CURSOR_ACCEPT) ? frameFill.background : frameFill.foreground;
		acceptBackground = (promptSelection == BOOL_PROMPT_CURSOR_ACCEPT) ? frameFill.foreground : frameFill.background;
		declineForeground = (promptSelection == BOOL_PROMPT_CURSOR_DECLINE) ? frameFill.background : frameFill.foreground;
		declineBackground = (promptSelection == BOOL_PROMPT_CURSOR_DECLINE) ? frameFill.foreground : frameFill.background;
		DrawTerminalGuiButton(acceptStart, optionPosY, promptAcceptText, acceptLength, acceptBackground, acceptForeground);
		DrawTerminalGuiButton(declineStart, optionPosY, promptDeclineText, declineLength, declineBackground, declineForeground);
	}
}

#endif