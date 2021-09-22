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

	// �����?
	// TODO: ���������� ��� � �� ������� �������� "�����"
	bool IsAlive() const noexcept { return GetHP() > 0; }
	// ����� �����������?
	// TODO: ��������� ��������� ������ ���� ����������
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

	Hero* grid[3][2] = { nullptr }; // ������������ ������. [1-3][0] - ������ ���; [1-3][1] - ������ ���
	std::vector<Hero> heroes;
};