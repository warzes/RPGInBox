#include "stdafx.h"
#include "Action.h"

Action::Action(Skill& skill, Unit& actingUnit)
	: skill(skill), actingUnit(actingUnit)
{

}

/*void Action::addTargets(std::vector<Unit> &targets)
{
	for (auto &t : targets)
	{
		targets.push_back(t);
	}
}*/

void Action::addTarget(Unit* target)
{
	targets.push_back(target);
}

void Action::clearTargets()
{
	targets = {};
}

std::wstring Action::getText() const
{
	return text;
}

void Action::execute()
{
	//std::cout << "executing action\n";

	if (targets.size() < 1)
	{
		//std::cerr << "error no action targets" << std::endl;
		return;
	}

	// for generating the action text
	// TODO implement different text for multitarget attacks
	text = actingUnit.name + L" " + skill.text + L" " + targets[0]->name + L"\n";
	int damage = targets[0]->currentStats.hp;

	switch (skill.id)
	{
	case Skill::ID::BasicAttack:
		executeBasicAttack();
		break;

	case Skill::ID::Cleave:
		executeCleave();
		break;

	case Skill::ID::Fireball:
		executeFireball();
		break;

	case Skill::ID::Firewave:
		executeFirewave();
		break;

	case Skill::ID::DoNothing:
		executeDoNothing();
		break;

	default:
		//std::cerr << "error no action name" << std::endl;
		break;
	}

	damage -= targets[0]->currentStats.hp;
	text += L"Dealt " + std::to_wstring(damage) + L" damage";
}

void Action::executeBasicAttack() const
{
	// TODO improve this damage formula

	for (size_t i = 0; i < targets.size(); ++i)
	{
		targets[i]->currentStats.hp -= actingUnit.currentStats.strength;
	}
}

void Action::executeFirewave() const
{
	// TODO improve this damage formula

	for (size_t i = 0; i < targets.size(); ++i)
	{
		targets[i]->currentStats.hp -= actingUnit.currentStats.intelligence;
	}
}

void Action::executeFireball() const
{
	// TODO improve this damage formula

	for (size_t i = 0; i < targets.size(); ++i)
	{
		targets[i]->currentStats.hp -= actingUnit.currentStats.intelligence + 5;
	}
}

void Action::executeCleave() const
{
	// TODO improve this damage formula

	for (size_t i = 0; i < targets.size(); ++i)
	{
		targets[i]->currentStats.hp -= actingUnit.currentStats.strength + 5;
	}
}

void Action::executeDoNothing() const
{
	return;
}