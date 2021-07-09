#include "stdafx.h"
#include "EnemyUnit.h"

EnemyUnit::EnemyUnit()
{
	totalStats = Stats();
	currentStats = totalStats;

	friendly = false;
	dead = false;
}

EnemyUnit::EnemyUnit(std::wstring name, Stats stats)
{
	totalStats = stats;
	currentStats = totalStats;
	this->name = name;
	friendly = false;
	dead = false;
}

//Action EnemyUnit::getNextAction()
//{
//
//}