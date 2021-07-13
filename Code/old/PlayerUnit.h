#pragma once

#include "Unit.h"

// a player unit, mostly a data class
struct PlayerUnit : public Unit
{
    //irr::video::ITexture* portrait;
    PlayerUnit();
    PlayerUnit(std::wstring name, Stats stats);
};