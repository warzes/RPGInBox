#pragma once

#include "Enemy.h"
#include "EngineMath.h"

/*
* ������� ����� ������� � ������ ��� � �����, ��� �������� ��������� �� �� ��������
*/
class EnemyParty
{
public:
	std::vector<oEnemy> enemys;
	std::vector<Point2> positionCharactersInParty;
};