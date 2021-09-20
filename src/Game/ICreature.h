#pragma once

class ICreature
{
public:
	virtual ~ICreature() = default;

	virtual int GetHP() const noexcept { return hp; }

	int hp = 10;
};