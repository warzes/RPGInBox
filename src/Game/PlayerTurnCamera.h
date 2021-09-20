#pragma once

#include "IGameCamera.h"

class Map;

class PlayerTurnCamera final : public IGameCamera
{
public:
	void Setup(const float fovY, const Vector3& position, const float currentRotateY = 0.0f) noexcept;
	void Update(const WorldAdventure& world) noexcept final;

	bool IsProcessMoving() const noexcept { return m_isMoving; }

private:
	enum class rotateDirY
	{
		Left,
		No,
		Right
	};

	enum class moveDir
	{
		No,
		Forward,
		Back,
		Left,
		Right
	};

	bool keyDown(int key) noexcept
	{
		return m_continuedWalk && !m_isMoving && !m_isTurning ? IsKeyDown(key) : IsKeyReleased(key);
	}

	bool isBlocked(const Vector3& newPosition, const Map& map) noexcept;

	void move(moveDir dir, const Map& map) noexcept;

	void turn(const rotateDirY dir) noexcept
	{
		if (!m_isTurning && dir != rotateDirY::No)
		{
			m_rotateDir = dir;
			m_isTurning = true;

			if (dir == rotateDirY::Left && m_currentRotateY <= 0.0f) m_currentRotateY = 360.0f;
			else if (dir == rotateDirY::Right && m_currentRotateY >= 360.0f) m_currentRotateY = 0.0f;

			if (dir == rotateDirY::Left)
				m_endRotateY = m_currentRotateY - 90.0f;
			else if (dir == rotateDirY::Right)
				m_endRotateY = m_currentRotateY + 90.0f;
		}
	}

	bool m_continuedWalk = true;
	float m_walkSpeed = 6.0f;
	float m_stepSize = 1.0f;

	bool m_isMoving = false;
	bool m_isTurning = false;

	Vector3 m_targetPosition = { 0.0f, 0.0f, 0.0f };
	moveDir m_moveDir = moveDir::No;
	Vector3 m_direction = { 0 };

	rotateDirY m_rotateDir = rotateDirY::No;
	float m_endRotateY = 0.0f;
	float m_speedRotate = 200.0f;
};