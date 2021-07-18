#pragma once

#include "PlayerParty.h"
#include "Map.h"

class World final : NonCopyable, NonMovable
{
public:
	~World();
	bool Init();

	void Move(const Point2& pos) noexcept;

	PlayerParty playerParty;
	Map openworld;
private:		
};