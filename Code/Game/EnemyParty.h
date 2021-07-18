#pragma once

#include "Enemy.h"

/*
* монстры также собраны в группу как и герои, это позволит подбирать их по тематике
*/
class EnemyParty
{
public:
	std::vector<Enemy> enemys;
};