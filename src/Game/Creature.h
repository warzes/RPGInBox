#pragma once

enum class CreatureType
{
	Hero,
	Enemy
};

class ICreature
{
public:
	virtual ~ICreature() = default;

	virtual int GetHP() const noexcept { return hp; }
	virtual CreatureType GetType() const noexcept = 0;

	// живой?
	// TODO: сравнивать еще и со стейтом существа "мертв"
	bool IsAlive() const noexcept { return GetHP() > 0; }
	// может действовать?
	// TODO: проверять остальные стейты типа окаменения
	bool IsAction() const noexcept { return true; }

	int hp = 10;
};

class Hero final : public ICreature
{
public:
	CreatureType GetType() const noexcept final { return CreatureType::Hero; }
};

class Player final
{
public:
	void CreateDefaultParty() noexcept;

	Hero* grid[3][2] = { nullptr }; // расположение героев. [1-3][0] - первый ряд; [1-3][1] - второй ряд
	std::vector<Hero> heroes;
};