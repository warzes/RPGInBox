#include "stdafx.h"
#include "BattleRule.h"
#include "Creature.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void BattleRule::MeleeDamage(ICreature& attacking, ICreature& defending) noexcept
{
	defending.Damage(5);
}
//-----------------------------------------------------------------------------