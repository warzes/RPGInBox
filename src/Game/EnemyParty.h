#pragma once

#include "Enemy.h"
#include "EngineMath.h"

/*
* монстры также собраны в группу как и герои, это позволит подбирать их по тематике
*/
class oEnemyParty
{
public:
	std::vector<oEnemy> enemys;
	std::vector<Point2> positionCharactersInParty;
};