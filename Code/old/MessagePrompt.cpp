#include "stdafx.h"
#if ASCII_DEFINE
#include "View.h"

#include "Input.h"
#include "terminaldraw.h"
#include "terminalgui.h"
#include "terminalwrite.h"

#define MESSAGE_PROMPT_DEFAULT_ACCEPT "Accept"

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

static const View MESSAGE_PROMPT_VIEW = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = false
};

static bool promptIsOpen;
static const char* promptTitleText;
static const char* promptMessageText;
static const char* promptAcceptText;

void OpenMessagePrompt(const char* titleText, const char* messageText, const char* acceptText)
{
	// CRASH: Only one prompt allowed at a time
	if (promptIsOpen) {
		TraceLog(LOG_ERROR, "PROMPT: MessagePrompt is already open");
		return;
	}

	promptIsOpen = true;
	promptTitleText = titleText;
	promptMessageText = messageText;
	promptAcceptText = (acceptText) ? acceptText : MESSAGE_PROMPT_DEFAULT_ACCEPT;

	PushView(&MESSAGE_PROMPT_VIEW);
}

static void OnOpenView(void)
{
	// Do nothing
}

static void OnCloseView(void)
{
	promptIsOpen = false;
}

static void OnControlView(void)
{
	if (IsInputActive(INPUT_UI_SUBMIT))
		PopView();
}

// TODO: Support messages that are so long, they have to wrap to another screen,
// which can be interacted with use LEFT/RIGHT, maybe even HOME/END
static void OnRenderView(void)
{
	static const int frameWidth = 60;
	static const int frameHeight = 20;
	static const int maxMessageHeight = frameHeight - 8;
	static const TerminalTile frameFill = { .background = BLACK, .foreground = WHITE, .symbol = ' ' };
	static const TerminalTile frameOutline = { .foreground = DARKGRAY, .symbol = (char)203 };
	static const TerminalTile frameOutlineTitle = { .background = GRAY, .foreground = BLACK };
	int framePosX;
	int framePosY;

	// Layout frame
	framePosX = (GetTerminalWidth() - frameWidth) / 2;
	framePosY = (GetTerminalHeight() - frameHeight) / 2;
	DrawTerminalGuiFrame(framePosX, framePosY, frameWidth, frameHeight, promptTitleText, frameFill, frameOutline, frameOutlineTitle);

	// Layout message
	DrawTerminalGuiTextArea(framePosX + 2, framePosY + 2, frameWidth - 4, frameHeight - 5, promptMessageText, ALPHA_BLACK, WHITE);

	// Layout options
	{
		int acceptLength;
		int acceptPosX;
		int acceptPosY;

		acceptLength = strlen(promptAcceptText);
		acceptPosX = framePosX + ((frameWidth - acceptLength - 2) / 2);
		acceptPosY = framePosY + frameHeight - 3;
		MoveTerminalCursorDown(1);
		DrawTerminalGuiButton(acceptPosX, acceptPosY, promptAcceptText, acceptLength, WHITE, BLACK);
	}
}
#endif