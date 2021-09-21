#pragma once

#include "ICreature.h"

class Heroes final : public ICreature
{
public:

};

class HeroesParty final
{
public:
	std::vector<Heroes> heroes;
};

class Player final
{
public:
	void StartDefaultParty() noexcept;

	HeroesParty& GetParty() noexcept { return m_party; }
private:
	HeroesParty m_party;
};