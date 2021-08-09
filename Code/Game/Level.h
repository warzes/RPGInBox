#pragma once

#include "IGameModule.h"

class Level final : public IGameModule, public IPlayfield
{
public:
	Level();
	void Init(int mapId, int startCol, int startRow, int inRoom);
};