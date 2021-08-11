#pragma once

#include "Global.h"

namespace Player
{
	struct MagicAttr;
}

typedef void (*CalcMagicFunc)(const Player::MagicAttr& magicAttr, Actor* target, ActionResult& result);

CalcMagicFunc GetMagicProc(int effect);