#pragma once

/* идея тут в том что в классе все статы, но возможно в будущем разбить*/

class Stats final
{
public:
	Stats(int strength, int intelligence, int faith, int vitality, int agility, int hp)
		: strength(strength), intelligence(intelligence), faith(faith), agility(agility), hp(hp)
	{
	}

	Stats()
	{
	}

	// Prime Attributes
	int strength = 1, intelligence = 1, faith = 1, vitality = 1, agility = 1; 

	// stats
	int hp = 1;
};