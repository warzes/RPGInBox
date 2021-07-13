#include "stdafx.h"
#include "Skills.h"
#include <Engine/DebugNew.h>

Skill::Skill(ID id, TargetingID targeting, std::wstring name, std::wstring text)
	: id(id), targeting(targeting), name(name), text(text)
{
}

Skill Skills::basicAttack = Skill(Skill::ID::BasicAttack, Skill::TargetingID::Single, L"BasicAttack", L"attacks");
Skill Skills::cleave = Skill(Skill::ID::Cleave, Skill::TargetingID::Single, L"Cleave", L"cleaves");
Skill Skills::fireball = Skill(Skill::ID::Fireball, Skill::TargetingID::Single, L"Fireball", L"casts fireball on");
Skill Skills::firewave = Skill(Skill::ID::Firewave, Skill::TargetingID::Single, L"Firewave", L"casts firewave on");
Skill Skills::doNothing = Skill(Skill::ID::DoNothing, Skill::TargetingID::Single, L"DoNothing", L"does nothing to");