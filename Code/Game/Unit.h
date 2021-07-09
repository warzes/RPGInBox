#pragma once

#include "Stats.h"
#include "Skills.h"

struct Unit
{
public:
	std::wstring name;

	Stats totalStats;
	Stats currentStats;
	bool friendly;
	bool dead;

	//irr::gui::IGUIStaticText* hpText;
	//irr::video::ITexture* fullbodyImage;

	std::vector<Skill*> skills;

	// a comparator class so we can put units in a priority queue
	struct CompareSpeed
	{
		bool operator()(Unit* unit1, Unit* unit2)
		{
			// TODO when we add secondary stats this will need to be updated to use those
			return unit1->currentStats.agility < unit2->currentStats.agility;
		}
	};

	Unit();
	Unit(std::wstring name, Stats stats);
	~Unit();

	// return if the unit is a player unit or not
	bool isFriendly();

	void addSkill(Skill& skill);
};