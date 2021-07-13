#include "stdafx.h"
#if ASCII_DEFINE
#include "View.h"

#include "Creature.h"
#include "Handle.h"
#include "Input.h"
#include "Inventory.h"
#include "Item.h"
#include "terminaldraw.h"
#include "terminalgui.h"
#include "terminalwrite.h"

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

const View VIEW_GAME_INVENTORY = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = true
};

static void OnOpenView(void)
{
	// Do nothing
}

static void OnCloseView(void)
{
	// Do nothing
}

static void OnControlView(void)
{
	if (IsInputActive(INPUT_UI_CANCEL))
		PopView();
}

static void OnRenderView(void)
{
	static const int framePaddingLeft = 2;
	static const int framePaddingTop = 2;
	static const TerminalTile frameFill = { .background = BLACK, .foreground = WHITE, .symbol = ' ' };
	static const TerminalTile frameOutline = { .foreground = DARKGRAY, .symbol = (char)203 };
	static const TerminalTile frameTitleTile = { .background = GRAY, .foreground = BLACK };
	int i;
	Handle inventory;

	inventory = GetCreatureInventory(GetCreatureProtagonist());

	// Draw frame
	DrawTerminalGuiFrame(0, 0, GetTerminalWidth(), GetTerminalHeight(), "Inventory", frameFill, frameOutline, frameTitleTile);

	// Draw items
	SetTerminalCursorWrap(false, false);
	SetTerminalCursorXY(0, 0);
	SetTerminalWriteBackPaint(BLACK);


	for (i = 0; i < MAX_INVENTORY_ITEM_QUANTITY; i++) {
		ItemPrefab item;

		item = GetInventoryItemAtIndex(inventory, i);

		if (item != ITEM_PREFAB_NONE) {
			SetTerminalCursorXY(framePaddingLeft, GetTerminalCursorY() + framePaddingTop);
			SetTerminalWriteForePaint(WHITE);
			WriteTerminalText(TextFormat("%s (%d gp) (%.2f kg)", GetItemName(item), GetItemPrice(item), GetItemWeight(item)));
			SetTerminalWriteForePaint(GRAY);
			SetTerminalCursorXY(framePaddingLeft, GetTerminalCursorY() + 1);
			WriteTerminalText(GetItemDescription(item));
		}
	}
}
#endif