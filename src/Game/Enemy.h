#pragma once

#include "Stats.h"

class Enemy
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