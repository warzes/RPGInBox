#pragma once

constexpr size_t MapSize = 20;

class Party final : NonCopyable, NonMovable
{
public:
	Point2 GetPosition() const noexcept { return m_position; }
	void SetPosition(const Point2& pos) noexcept { m_position = m_oldPosition = pos; }

	bool Move(const Vector3& pos) noexcept;

private:
	Point2 m_position;
	Point2 m_oldPosition;
};

class Tile final : NonCopyable, NonMovable
{
public:
	bool isTree = false;
};

class Map final : NonCopyable, NonMovable
{
public:
	void InitTest() noexcept;

	Tile tiles[MapSize][MapSize];
};

class World final : NonCopyable, NonMovable
{
public:
	~World();
	bool Init();

	void Move(const Vector3& pos) noexcept;

	Party playerParty;
	Map openworld;
private:
		
};