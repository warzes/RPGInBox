#pragma once

class ResourceManager;

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

	std::shared_ptr<Texture2D> battleTexture = nullptr;

	int hp = 10;
};

class Hero final : public ICreature
{
public:
	CreatureType GetType() const noexcept final { return CreatureType::Hero; }
};

class Enemy final : public ICreature
{
public:
	CreatureType GetType() const noexcept final { return CreatureType::Enemy; }
};

// нет отдельного класса под партию - не нужен, передавать сразу игрока
class Player final
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr) noexcept;


	Hero* grid[3][2] = { nullptr }; // расположение героев. [1-3][0] - первый ряд; [1-3][1] - второй ряд
	std::vector<Hero> heroes;
};

// а враг выделен в партию, потому что он временный, только для одного боя
class EnemyParty final
{
public:
	void CreateDefaultParty(ResourceManager* resourceMgr) noexcept;

	Enemy* grid[3][2] = { nullptr }; // расположение врагов. [1-3][0] - первый ряд; [1-3][1] - второй ряд
	std::vector<Enemy> enemys;
};