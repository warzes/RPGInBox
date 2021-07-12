#include "stdafx.h"
#if ASCII_DEFINE
#include "Input.h"

typedef enum {
	INPUT_KEY_STATE_NONE,
	INPUT_KEY_STATE_PRESSED,
	INPUT_KEY_STATE_DOWN,
	INPUT_KEY_STATE_DOWN_PULSE,
	INPUT_KEY_STATE_RELEASED
} InputKeyState;

static double inputPulseMinDownTime;
static double inputPulseCooldown;
static struct {
	struct {
		int primaryKey;
		int secondaryKey;
		InputKeyState state;
		bool holdAlt;
		bool holdControl;
		bool holdShift;
	} bindInfo;
	struct {
		InputKeyState state;
		double downTime;
		double pulseTimer;
		bool active;
	} stateInfo;
} inputInfo[INPUT__LENGTH];

void InitInput(void)
{
	LoadDefaultInputConfig();
}

bool IsInputActive(Input input)
{
	return inputInfo[input].stateInfo.active;
}

void LoadDefaultInputConfig(void)
{
	inputPulseMinDownTime = 0.33L;
	inputPulseCooldown = 0.065L;

	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.primaryKey = KEY_SPACE;
	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.secondaryKey = 0;
	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.state = INPUT_KEY_STATE_PRESSED;
	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.holdAlt = false;
	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.holdControl = false;
	inputInfo[INPUT_GAME_FLOOR_PICK_UP].bindInfo.holdShift = false;

	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.primaryKey = KEY_UP;
	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.secondaryKey = KEY_W;
	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.holdAlt = false;
	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.holdControl = false;
	inputInfo[INPUT_GAME_WALK_NORTH].bindInfo.holdShift = false;

	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.primaryKey = KEY_RIGHT;
	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.secondaryKey = KEY_D;
	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.holdAlt = false;
	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.holdControl = false;
	inputInfo[INPUT_GAME_WALK_EAST].bindInfo.holdShift = false;

	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.primaryKey = KEY_DOWN;
	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.secondaryKey = KEY_S;
	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.holdAlt = false;
	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.holdControl = false;
	inputInfo[INPUT_GAME_WALK_SOUTH].bindInfo.holdShift = false;

	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.primaryKey = KEY_LEFT;
	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.secondaryKey = KEY_A;
	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.holdAlt = false;
	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.holdControl = false;
	inputInfo[INPUT_GAME_WALK_WEST].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_UP].bindInfo.primaryKey = KEY_UP;
	inputInfo[INPUT_UI_UP].bindInfo.secondaryKey = KEY_W;
	inputInfo[INPUT_UI_UP].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_UI_UP].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_UP].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_UP].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_RIGHT].bindInfo.primaryKey = KEY_RIGHT;
	inputInfo[INPUT_UI_RIGHT].bindInfo.secondaryKey = KEY_D;
	inputInfo[INPUT_UI_RIGHT].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_UI_RIGHT].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_RIGHT].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_RIGHT].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_DOWN].bindInfo.primaryKey = KEY_DOWN;
	inputInfo[INPUT_UI_DOWN].bindInfo.secondaryKey = KEY_S;
	inputInfo[INPUT_UI_DOWN].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_UI_DOWN].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_DOWN].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_DOWN].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_LEFT].bindInfo.primaryKey = KEY_LEFT;
	inputInfo[INPUT_UI_LEFT].bindInfo.secondaryKey = KEY_A;
	inputInfo[INPUT_UI_LEFT].bindInfo.state = INPUT_KEY_STATE_DOWN_PULSE;
	inputInfo[INPUT_UI_LEFT].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_LEFT].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_LEFT].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_SUBMIT].bindInfo.primaryKey = KEY_ENTER;
	inputInfo[INPUT_UI_SUBMIT].bindInfo.secondaryKey = KEY_E;
	inputInfo[INPUT_UI_SUBMIT].bindInfo.state = INPUT_KEY_STATE_PRESSED;
	inputInfo[INPUT_UI_SUBMIT].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_SUBMIT].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_SUBMIT].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_CANCEL].bindInfo.primaryKey = KEY_ESCAPE;
	inputInfo[INPUT_UI_CANCEL].bindInfo.secondaryKey = KEY_TAB;
	inputInfo[INPUT_UI_CANCEL].bindInfo.state = INPUT_KEY_STATE_PRESSED;
	inputInfo[INPUT_UI_CANCEL].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_CANCEL].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_CANCEL].bindInfo.holdShift = false;

	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.primaryKey = KEY_TAB;
	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.secondaryKey = 0;
	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.state = INPUT_KEY_STATE_PRESSED;
	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.holdAlt = false;
	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.holdControl = false;
	inputInfo[INPUT_UI_TOGGLE_INVENTORY].bindInfo.holdShift = false;
}

void UpdateInput(void)
{
	int i;
	bool altHeld;
	bool controlHeld;
	bool shiftHeld;
	double frameTime;

	altHeld = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
	controlHeld = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
	shiftHeld = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
	frameTime = GetFrameTime();

	for (i = 0; i < INPUT__LENGTH; i++) {
		bool down;
		bool pressed;
		bool released;
		bool pulsed;

		// Check keybind activity
		down = IsKeyDown(inputInfo[i].bindInfo.primaryKey) || IsKeyDown(inputInfo[i].bindInfo.secondaryKey);
		pressed = IsKeyPressed(inputInfo[i].bindInfo.primaryKey) || IsKeyPressed(inputInfo[i].bindInfo.secondaryKey);
		released = IsKeyReleased(inputInfo[i].bindInfo.primaryKey) || IsKeyReleased(inputInfo[i].bindInfo.secondaryKey);

		// Check mod key activity. Note, multiplier + boolean means the 
		// value remains active when true and toggled off when false
		if (inputInfo[i].bindInfo.holdAlt) {
			down *= altHeld;
			pressed *= altHeld;
			released *= altHeld;
		}

		if (inputInfo[i].bindInfo.holdControl) {
			down *= controlHeld;
			pressed *= controlHeld;
			released *= controlHeld;
		}

		if (inputInfo[i].bindInfo.holdShift) {
			down *= shiftHeld;
			pressed *= shiftHeld;
			released *= shiftHeld;
		}

		// Update key state
		if (down)
			inputInfo[i].stateInfo.state = INPUT_KEY_STATE_DOWN;
		else if (pressed)
			inputInfo[i].stateInfo.state = INPUT_KEY_STATE_PRESSED;
		else if (released)
			inputInfo[i].stateInfo.state = INPUT_KEY_STATE_RELEASED;
		else
			inputInfo[i].stateInfo.state = INPUT_KEY_STATE_NONE;

		// Reset active state
		inputInfo[i].stateInfo.active = false;
		pulsed = false;

		if (down) {
			inputInfo[i].stateInfo.downTime += frameTime;
			inputInfo[i].stateInfo.pulseTimer += frameTime;

			if (inputInfo[i].stateInfo.pulseTimer > inputPulseCooldown) {
				if (inputInfo[i].stateInfo.downTime > inputPulseMinDownTime)
					pulsed = true;

				inputInfo[i].stateInfo.pulseTimer = 0L;
			}
		} if (released) {
			inputInfo[i].stateInfo.downTime = 0;
			inputInfo[i].stateInfo.pulseTimer = 0;
		}

		// Update active state
		if (inputInfo[i].bindInfo.state == INPUT_KEY_STATE_DOWN_PULSE) {
			if (pressed)
				inputInfo[i].stateInfo.active = true;
			else if (pulsed && down)
				inputInfo[i].stateInfo.active = true;
		}
		else if (inputInfo[i].bindInfo.state == INPUT_KEY_STATE_PRESSED) {
			if (pressed)
				inputInfo[i].stateInfo.active = true;
		}
		else if (inputInfo[i].bindInfo.state == INPUT_KEY_STATE_DOWN) {
			if (down)
				inputInfo[i].stateInfo.active = true;
		}
		else if (inputInfo[i].bindInfo.state == INPUT_KEY_STATE_RELEASED) {
			if (released)
				inputInfo[i].stateInfo.active = true;
		}
	}
}
#endif