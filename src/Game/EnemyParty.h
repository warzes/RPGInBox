#pragma once

#include "Enemy.h"
#include "EngineMath.h"

/*
* ������� ����� ������� � ������ ��� � �����, ��� �������� ��������� �� �� ��������
*/
class oEnemyParty
{
public:
	std::vector<oEnemy> enemys;
	std::vector<Point2> positionCharactersInParty;
};