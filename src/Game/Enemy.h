#pragma once

#include "Stats.h"
#include "ICreature.h"

class Enemy final : public ICreature
{
public:
	std::string name;

	// Statistics
	struct
	{
		Stats hp = 0;
		Stats mp = 0;
	} statistics;
};