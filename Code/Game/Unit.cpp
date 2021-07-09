#include "stdafx.h"
#include "Unit.h"
#include <Engine/DebugNew.h>

Unit::Unit()
	: friendly(false)
{
	totalStats = Stats();
	currentStats = totalStats;
	name = L"";
	dead = true;
}

Unit::Unit(std::wstring name, Stats stats)
	: name(name), totalStats(stats), friendly(false)
{
	currentStats = totalStats;
	dead = true;
}

Unit::~Unit()
{
}

bool Unit::isFriendly()
{
	return friendly;
}

void Unit::addSkill(Skill& skill)
{
	for (size_t i = 0; i < skills.size(); ++i)
	{
		if (skills[i]->id == skill.id)
		{
			return;
		}
	}
	skills.push_back(&skill);
}