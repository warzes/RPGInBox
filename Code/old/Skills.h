#pragma once

struct Skill
{
	enum class ID
	{
		BasicAttack,
		Cleave,
		Fireball,
		Firewave,
		DoNothing
	};

	enum class TargetingID
	{
		Single,
		All
	};

	Skill(ID id, TargetingID targeting, std::wstring name, std::wstring text);

	std::wstring name;
	std::wstring text;

	ID id;
	TargetingID targeting;
};

namespace Skills
{
	// TODO: const
	extern Skill basicAttack;
	extern Skill cleave;
	extern Skill fireball;
	extern Skill firewave;
	extern Skill doNothing;
}