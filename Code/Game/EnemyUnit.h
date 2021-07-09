#pragma once

#include "Unit.h"
#include "Action.h"

// a enemy unit, mostly a data class
struct EnemyUnit :
    public Unit
{

    EnemyUnit();
    EnemyUnit(std::wstring name, Stats stats);

    //Action getNextAction();
};