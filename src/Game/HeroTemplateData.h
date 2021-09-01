#pragma once

#include "Hero.h"

class HeroTemplateData final
{
public:
	static Hero CreateWarrior(const char* name) noexcept;
};