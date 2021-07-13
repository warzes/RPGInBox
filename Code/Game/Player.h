#pragma once

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