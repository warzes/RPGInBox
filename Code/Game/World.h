#pragma once

#include "Player.h"
#include "Map.h"

class World final : NonCopyable, NonMovable
{
public:
	~World();
	bool Init();

	void Move(const Vector3& pos) noexcept;

	Party playerParty;
	Map openworld;
private:		
};