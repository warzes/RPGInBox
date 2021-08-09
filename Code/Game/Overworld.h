#pragma once

#include "IGameModule.h"

class Overworld final : public IGameModule, public IPlayfield
{
public:
	Overworld();

	void Init(int startCol, int startRow);
};