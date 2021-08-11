#include "stdafx.h"
#include "Input.h"

bool Input::IsKeyDown(int keyCode)
{
	return ::IsKeyDown(keyCode);
}

bool Input::IsKeyPressing(int keyCode)
{
	return ::IsKeyPressed(keyCode);
}

Direction Input::GetInputDirection()
{
	if (IsKeyDown(KEY_LEFT))       return Dir_Left;
	else if (IsKeyDown(KEY_RIGHT)) return Dir_Right;
	else if (IsKeyDown(KEY_UP))    return Dir_Up;
	else if (IsKeyDown(KEY_DOWN))  return Dir_Down;
	return Dir_None;
}