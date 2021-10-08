#pragma once

class ICreature;

class BattleRule final
{
public:
	static void MeleeDamage(ICreature& attacking, ICreature& defending) noexcept;
};