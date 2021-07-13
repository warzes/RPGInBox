#pragma once

#include "Unit.h"
#include "Skills.h"

// An action used in battle
class Action
{
	// action text 
	std::wstring text = L"";

	// functions for each different actions executions

	void executeBasicAttack() const;

	void executeFirewave() const;

	void executeFireball() const;

	void executeCleave() const;

	void executeDoNothing() const;

public:
	// data, this is pretty much a data class so we made these public
	Unit& actingUnit;
	std::vector<Unit*> targets;

	Action(Skill& skill, Unit& actingUnit);

	// the skill
	Skill& skill;

	// execute the action 
	void execute();

	// add to targets
//	void addTargets(std::vector<Unit> &targets);

	// add the target
	void addTarget(Unit* target);

	// clear targets
	void clearTargets();

	// get the actionText generated during the execute() function
	std::wstring getText() const;

	// a comparator class so we can put actions in a priority queue
	struct CompareSpeed
	{
		bool operator()(Action& a1, Action& a2)
		{
			// TODO when we add secondary stats this will need to be updated to use those
			return a1.actingUnit.currentStats.agility < a2.actingUnit.currentStats.agility;
		}
		bool operator()(Action* a1, Action* a2)
		{
			// TODO when we add secondary stats this will need to be updated to use those
			return (*a1).actingUnit.currentStats.agility < (*a2).actingUnit.currentStats.agility;
		}
	};
};