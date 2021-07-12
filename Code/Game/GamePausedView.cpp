#include "stdafx.h"
#include "View.h"

#include "Compass.h"
#include "Creature.h"
#include "Input.h"
#include "Prompt.h"
#include "terminaldraw.h"
#include "terminalwrite.h"

typedef enum {
	PAUSED_GAME_CURSOR_CONTINUE,
	PAUSED_GAME_CURSOR_NEW_GAME,
	PAUSED_GAME_CURSOR_LOAD_GAME,
	PAUSED_GAME_CURSOR_SAVE_GAME,
	PAUSED_GAME_CURSOR_HELP,
	PAUSED_GAME_CURSOR_QUIT
} PausedGameCursor;

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

const View VIEW_GAME_PAUSED = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = false
};

static PausedGameCursor pausedGameCursor;

static void OnOpenView(void)
{
	pausedGameCursor = PAUSED_GAME_CURSOR_CONTINUE;
}

static void OnCloseView(void)
{
	// Do nothing
}

static void OnControlView(void)
{
	if (IsInputActive(INPUT_UI_CANCEL)) {
		PopView();
	}
	else if (IsInputActive(INPUT_UI_UP) && pausedGameCursor > PAUSED_GAME_CURSOR_CONTINUE) 
	{
		int t = pausedGameCursor;
		t--;
		pausedGameCursor = (PausedGameCursor)t;
	}
	else if (IsInputActive(INPUT_UI_DOWN) && pausedGameCursor < PAUSED_GAME_CURSOR_QUIT) 
	{
		int t = pausedGameCursor;
		t++;
		pausedGameCursor = (PausedGameCursor)t;
	}
	else if (IsInputActive(INPUT_UI_SUBMIT)) {
		if (pausedGameCursor == PAUSED_GAME_CURSOR_CONTINUE)
			PopView();
		else if (pausedGameCursor == PAUSED_GAME_CURSOR_NEW_GAME)
			OpenNewGamePrompt();
		else if (pausedGameCursor == PAUSED_GAME_CURSOR_QUIT)
			OpenQuitGamePrompt();
	}
}

static void OnRenderView(void)
{
	static const int panelPosX = 3;
	static const int panelWidth = 22;
	static const TerminalTile panelTile = { .background = BLACK, .foreground = WHITE, .symbol = ' ' };
	static const char* const title = "[ Paused ]";
	static const int titleLength = 10;
	static const Color selectedColor = WHITE;
	static const Color unselectedColor = GRAY;
	static const int leftPadding = (panelWidth - titleLength) / 2;

	// Layout panel
	DrawTerminalBoxFill(panelPosX, 0, panelWidth, GetScreenHeight(), panelTile);

	// Layout title
	SetTerminalCursorWrap(false, false);
	SetTerminalWriteBackPaint(ALPHA_BLACK);
	SetTerminalWriteForePaint(WHITE);
	SetTerminalCursorXY(leftPadding, 3);
	WriteTerminalText(title);

	// Layout continue option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 3);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_CONTINUE) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Continue");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Continue");
	}

	// Layout new game option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 2);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_NEW_GAME) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> New Game");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  New Game");
	}

	// Layout load game option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 2);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_LOAD_GAME) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Load Game");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Load Game");
	}

	// Layout save game option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 2);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_SAVE_GAME) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Save Game");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Save Game");
	}

	// Layout help option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 2);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_HELP) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Help");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Help");
	}

	// Layout quit option
	SetTerminalCursorXY(leftPadding, GetTerminalCursorY() + 2);

	if (pausedGameCursor == PAUSED_GAME_CURSOR_QUIT) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Quit");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Quit");
	}
}