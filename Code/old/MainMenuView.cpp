#include "stdafx.h"
#if ASCII_DEFINE
#include "View.h"

#include "Game.h"
#include "Input.h"
#include "Prompt.h"
#include "terminaldraw.h"
#include "terminalwrite.h"

typedef enum {
	MAIN_MENU_CURSOR_NEW_GAME,
	MAIN_MENU_CURSOR_LOAD_GAME,
	MAIN_MENU_CURSOR_HELP,
	MAIN_MENU_CURSOR_QUIT
} MainMenuCursor;

static const int MAIN_MENU_BANNER_WIDTH = 41;
static const int MAIN_MENU_BANNER_HEIGHT = 9;
static const unsigned char MAIN_MENU_BANNER[] =
"\x20\x20\x20\x20\x20\x20\x20\x20\x53\x20\x43\x20\x52\x20\x4F\x20\x4C\x20\x4C\x20\x20\x20\x4F\x20\x46\x20\x20\x20\x54\x20\x48\x20\x45\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\xCB\x20\x20\xCB\xCB\x20\x20\x20\xCB\xCF\xCC\xCF\xCB\x20\xCB\xCF\xCC\xCC\x20\x20\xCC\xCB\x20\xCB\xCC\xCC\xCC\xCC\x20\xCC\xCB\xCB\xCB\xCC\x20\x20"
"\xCB\x20\x20\x20\x20\x20\xCB\x20\xCB\x20\xCB\x20\x20\xCB\x20\xCB\x20\xCB\x20\xCB\x20\x20\x20\xCB\x20\xCB\xCD\x20\xCB\x20\x20\xCC\xCF\x20\xCB\xCF\x20\x20\x20\xCF\x20"
"\x20\xCB\x20\x20\x20\x20\xCB\x20\xCB\xCC\xCC\xCB\x20\xCB\x20\xCC\x20\xCB\x20\xCB\xCF\xCF\xCF\x20\x20\xCB\xCD\x20\xCB\xCF\xCF\xCD\x20\x20\xCB\xCB\xCC\xCC\x20\x20\x20"
"\x20\x20\xCB\x20\x20\xCB\x20\x20\xCB\x20\x20\xCB\x20\xCB\x20\x20\x20\xCB\x20\xCB\x20\x20\x20\x20\x20\xCE\xCB\x20\xCB\x20\x20\xCB\x20\x20\xCB\xCC\x20\x20\x20\xCC\xCF"
"\x20\x20\x20\xCB\xCB\x20\x20\x20\x20\x20\x20\xCB\x20\x20\x20\x20\xCB\x20\x20\x20\xCB\x20\x20\x20\x20\x20\xCE\x20\x20\x20\xCB\x20\x20\x20\xCF\xCB\xCB\xCB\xCF\x20\x20"
"\x20\x20\x20\xCE\x20\x20\x20\x20\x20\x20\xCB\x20\x20\x20\x20\xCF\x20\x20\x20\x20\x20\xCF\x20\x20\x20\x20\x20\x20\x20\xCF\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\xCF\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
;

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

const View VIEW_MAIN_MENU = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = true
};

static MainMenuCursor mainMenuCursor;

static void OnOpenView(void)
{
	mainMenuCursor = MAIN_MENU_CURSOR_NEW_GAME;
}

static void OnCloseView(void)
{
	// Do nothing
}

static void OnControlView(void)
{
	if (IsInputActive(INPUT_UI_CANCEL)) {
		OpenQuitPrompt();
	}
	else if (IsInputActive(INPUT_UI_UP)) {
		if (mainMenuCursor > MAIN_MENU_CURSOR_NEW_GAME)
		{
			int t = mainMenuCursor;
			t--;
			mainMenuCursor = (MainMenuCursor)t;
		}
	}
	else if (IsInputActive(INPUT_UI_DOWN)) {
		if (mainMenuCursor < MAIN_MENU_CURSOR_QUIT)
		{
			int t = mainMenuCursor;
			t++;
			mainMenuCursor = (MainMenuCursor)t;
		}
	}
	else if (IsInputActive(INPUT_UI_SUBMIT)) {
		switch (mainMenuCursor) {
		case MAIN_MENU_CURSOR_NEW_GAME:
			NewGame();
			SetView(&VIEW_GAME_DEFAULT);
			break;
		case MAIN_MENU_CURSOR_LOAD_GAME:
			TraceLog(LOG_INFO, "VIEW: MainMenu: Loading new game");
			break;
		case MAIN_MENU_CURSOR_HELP:
			OpenMessagePrompt("Help", "Press button. Bump red \"w\" 'til gone.", "Thanks!");
			break;
		case MAIN_MENU_CURSOR_QUIT:
			OpenQuitPrompt();
			break;
		default:
			break;
		}
	}
}

static void OnRenderView(void)
{
	static const int left = 3;

	// Terminal write setup
	SnapTerminalCursorBegin();
	SetTerminalCursorWrap(false, false);
	SetTerminalWriteBackPaint(BLACK);

	// Draw title
	SetTerminalWriteForePaint(WHITE);
	SetTerminalCursorXY(left, 3);
	WriteTerminalText("THE VAMPIRE PROJECT");

	// Draw "New Game" option
	SetTerminalCursorXY(left, GetTerminalCursorY() + 2);

	if (mainMenuCursor == MAIN_MENU_CURSOR_NEW_GAME) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> New Game");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  New Game");
	}

	// Draw "Load Game" option
	SetTerminalCursorXY(left, GetTerminalCursorY() + 1);

	if (mainMenuCursor == MAIN_MENU_CURSOR_LOAD_GAME) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Load Game");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Load Game");
	}

	// Draw "Help" option
	SetTerminalCursorXY(left, GetTerminalCursorY() + 1);

	if (mainMenuCursor == MAIN_MENU_CURSOR_HELP) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Help");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Help");
	}

	// Draw "Quit" option
	SetTerminalCursorXY(left, GetTerminalCursorY() + 2);

	if (mainMenuCursor == MAIN_MENU_CURSOR_QUIT) {
		SetTerminalWriteForePaint(WHITE);
		WriteTerminalText("> Quit");
	}
	else {
		SetTerminalWriteForePaint(GRAY);
		WriteTerminalText("  Quit");
	}

	// Draw banner
	{
		int i;
		int panelPosX;
		int panelPosY;
		int panelWidth;
		int panelHeight;
		int bannerLength;
		int bannerPosX;
		int bannerPosY;
		TerminalTile bannerTile;

		// Calculate panel info
		panelWidth = GetTerminalWidth() - 25;
		panelHeight = GetTerminalHeight();
		panelPosX = GetTerminalWidth() - panelWidth;
		panelPosY = 0;

		// Calculate banner info
		bannerLength = MAIN_MENU_BANNER_WIDTH * MAIN_MENU_BANNER_HEIGHT;
		bannerPosX = panelPosX + ((panelWidth - MAIN_MENU_BANNER_WIDTH) / 2);
		bannerPosY = panelPosY + ((panelHeight - MAIN_MENU_BANNER_HEIGHT) / 2);
		//		bannerPosY = 3;
		bannerTile = TerminalTile{ .background = PURPLE, .foreground = BLACK };

		// Render panel
		DrawTerminalBoxFill(panelPosX, panelPosY, panelWidth, panelHeight, bannerTile);

		// Render banner
		i = 0;

		while (MAIN_MENU_BANNER[i] != '\0') {
			int x;
			int y;

			x = bannerPosX + (i % MAIN_MENU_BANNER_WIDTH);
			y = bannerPosY + (i / MAIN_MENU_BANNER_WIDTH);
			bannerTile.symbol = MAIN_MENU_BANNER[i];
			SetTerminalTile(x, y, bannerTile);

			i++;
		}

		// Render info stamp
		SetTerminalCursorWrap(false, false);
		SetTerminalWriteBackPaint(bannerTile.background);
		SetTerminalWriteForePaint(bannerTile.foreground);

		SetTerminalCursorXY(GetTerminalWidth() - 10, GetTerminalHeight() - 4);
		WriteTerminalText("7DRL 2021");

		SetTerminalCursorXY(GetTerminalWidth() - 14, GetTerminalCursorY() + 1);
		WriteTerminalText("VERSION 0.7.0");

		SetTerminalCursorXY(GetTerminalWidth() - 25, GetTerminalCursorY() + 1);
		WriteTerminalText("CREATED BY ANDREW GASSON");
	}
}
#endif