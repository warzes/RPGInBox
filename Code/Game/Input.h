#pragma once

#include "Global.h"

class Input
{
public:
	static bool IsKeyDown(int keyCode);
	static bool IsKeyPressing(int keyCode);
	static Direction GetInputDirection();
};