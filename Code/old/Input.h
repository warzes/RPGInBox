#pragma once

#if ASCII_DEFINE
// TODO: Reconsider whether toggle pause menu input is actually necessary.
// Because "Cancel" is essentially a "quit out of open tab" button, it makes 
// sense that when play the game, escape is the quit out of game option.
// A pause menu toggle is therefore unnecessary.
typedef enum Input {
	INPUT_GAME_FLOOR_PICK_UP,
	INPUT_GAME_WALK_EAST,
	INPUT_GAME_WALK_NORTH,
	INPUT_GAME_WALK_SOUTH,
	INPUT_GAME_WALK_WEST,
	INPUT_UI_UP,
	INPUT_UI_RIGHT,
	INPUT_UI_DOWN,
	INPUT_UI_LEFT,
	INPUT_UI_SUBMIT,
	INPUT_UI_CANCEL,
	INPUT_UI_TOGGLE_INVENTORY,
	// Size
	INPUT__LENGTH
} Input;

void InitInput(void);
bool IsInputActive(Input input);
void LoadDefaultInputConfig(void);
void UpdateInput(void);

#endif