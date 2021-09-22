#pragma once

#include "Hero.h"

class HeroTemplateData final
{
public:
	static oHero CreateWarrior(const char* name) noexcept;
};