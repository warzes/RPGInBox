#include "stdafx.h"
#if ASCII_DEFINE
#include "View.h"

#include "Compass.h"
#include "Creature.h"
#include "Input.h"

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

const View VIEW_GAME_DEFAULT = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = false,
	.disableGameWorldRender = false
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
	if (IsInputActive(INPUT_UI_TOGGLE_INVENTORY))
		PushView(&VIEW_GAME_INVENTORY);
	else if (IsInputActive(INPUT_GAME_FLOOR_PICK_UP))
		CreaturePickUpFloorItem(GetCreatureProtagonist(), 0);
	else if (IsInputActive(INPUT_GAME_WALK_NORTH))
		CreatureWalkOrInteract(GetCreatureProtagonist(), COMPASS_NORTH);
	else if (IsInputActive(INPUT_GAME_WALK_EAST))
		CreatureWalkOrInteract(GetCreatureProtagonist(), COMPASS_EAST);
	else if (IsInputActive(INPUT_GAME_WALK_SOUTH))
		CreatureWalkOrInteract(GetCreatureProtagonist(), COMPASS_SOUTH);
	else if (IsInputActive(INPUT_GAME_WALK_WEST))
		CreatureWalkOrInteract(GetCreatureProtagonist(), COMPASS_WEST);
	else if (IsInputActive(INPUT_UI_CANCEL))
		PushView(&VIEW_GAME_PAUSED);
}

static void OnRenderView(void)
{
	// TODO: HUD
}
#endif