#pragma once

#include "Character.h"

constexpr unsigned MaxNumCharacterInPlayerParty = 4;

class PlayerParty
{
public:
	Character characters[MaxNumCharacterInPlayerParty];

	// statistics
	unsigned level = 1;
	unsigned experience = 0;

	// party talents

	// inventory
	// TODO:
};