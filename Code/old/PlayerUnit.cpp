#include "stdafx.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit()
{
	totalStats = Stats();
	currentStats = totalStats;

	friendly = true;
	dead = false;
}

PlayerUnit::PlayerUnit(std::wstring name, Stats stats)
{
	totalStats = stats;
	currentStats = totalStats;
	this->name = name;
	friendly = true;
	dead = false;
}