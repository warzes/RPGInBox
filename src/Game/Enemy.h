#pragma once

#include "Stats.h"

class Enemy
{
public:
	char name[MaxGameNameTextSize];

	// Statistics
	struct
	{
		Stats hp = 0;
		Stats mp = 0;
	} statistics;
};