#include "stdafx.h"
#include "PlayerTurnCamera.h"
#include "WorldAdventure.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
//constexpr Vector2 DirectionLookup[4] =
//{
//	{ 0.0f, 1.0f },  // forward
//	{ -1.0f, 0.0f }, // right
//	{ 0.0f, -1.0f }, // backward
//	{ 1.0f, 0.0f },  // left
//};
//-----------------------------------------------------------------------------
void PlayerTurnCamera::Setup(const float fovY, const Vector3& position, const float currentRotateY) noexcept
{
	SetCameraPosition(position, currentRotateY);

	m_viewCamera.up = { 0.0f, 1.0f, 0.0f };
	m_viewCamera.fovy = fovY;
	m_viewCamera.projection = CAMERA_PERSPECTIVE;
}
//-----------------------------------------------------------------------------
void PlayerTurnCamera::Update(const WorldAdventure& world) noexcept
{
	if (!m_isTurning)
	{
		if (!m_isMoving) // во время вращения камеры нельзя двигаться, поэтому условие вложенное
		{
			if (keyDown(GameKey::Up)) move(moveDir::Forward, world.openworld);
			if (keyDown(GameKey::Down)) move(moveDir::Back, world.openworld);
			if (keyDown(GameKey::StrafeLeft)) move(moveDir::Left, world.openworld);
			if (keyDown(GameKey::StrafeRight)) move(moveDir::Right, world.openworld);
		}

		if (keyDown(GameKey::Left)) turn(rotateDirY::Left);
		if (keyDown(GameKey::Right)) turn(rotateDirY::Right);
	}
	if (m_isMoving)
	{
		m_cameraPosition.x += m_walkSpeed * m_direction.x * GetFrameTime();
		m_cameraPosition.z += m_walkSpeed * m_direction.z * GetFrameTime();

		if ((m_direction.x > 0 && m_cameraPosition.x >= m_targetPosition.x) ||
			(m_direction.x < 0 && m_cameraPosition.x <= m_targetPosition.x) ||
			(m_direction.z > 0 && m_cameraPosition.z >= m_targetPosition.z) ||
			(m_direction.z < 0 && m_cameraPosition.z <= m_targetPosition.z))
		{
			m_isMoving = false;
			m_cameraPosition.x = m_targetPosition.x;
			m_cameraPosition.z = m_targetPosition.z;
		}
		//printf("CP = %f %f\n", m_cameraPosition.x, m_cameraPosition.z);
	}
	if (m_isTurning)
	{
		m_currentRotateY += m_speedRotate
			* (m_rotateDir == rotateDirY::Left ? -1.0f : 1.0f)
			* GetFrameTime();

		if ((m_rotateDir == rotateDirY::Right && m_currentRotateY >= m_endRotateY) ||
			(m_rotateDir == rotateDirY::Left && m_currentRotateY <= m_endRotateY))
		{
			m_isTurning = false;
			m_rotateDir = rotateDirY::No;
			m_currentRotateY = m_endRotateY;
		}
	}
	SetCameraPosition(m_cameraPosition, m_currentRotateY);
}
//-----------------------------------------------------------------------------
bool PlayerTurnCamera::isBlocked(const Vector3& newPosition, const Map& map) noexcept
{
	if (map.tiles[(size_t)newPosition.x][(size_t)newPosition.z].IsBlocked())
		return true;

	return false;
}
//-----------------------------------------------------------------------------
void PlayerTurnCamera::move(moveDir dir, const Map& map) noexcept
{
	// TODO: не работает - вращение всеже может быть меньше нуля или равно 360
	//int index = m_currentRotateY / 90.0f;
	//if (index < 0 || index > 3) return;
	//const auto &direction = DirectionLookup[index];
	//if (dir == moveDir::Forward) m_direction = { direction.x, 0.0f, direction.y };
	//else if (dir == moveDir::Back) m_direction = { -direction.x, 0.0f, -direction.y };
	//else if (dir == moveDir::Left) m_direction = { -direction.y, 0.0f, direction.x };
	//else if (dir == moveDir::Right) m_direction = { direction.y, 0.0f, -direction.x };

	Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
	forward = Vector3Normalize(forward);
	// это подсказки, не код
	// back { -forward.x, 0,-forward.z };
	// left { -forward.z, 0, forward.x };
	// right{  forward.z, 0,-forward.x };

	if (dir == moveDir::Forward)    m_direction = { round(forward.x),  0.0f, round(forward.z) };
	else if (dir == moveDir::Back)  m_direction = { round(-forward.x), 0.0f, round(-forward.z) };
	else if (dir == moveDir::Right) m_direction = { round(-forward.z), 0.0f, round(forward.x) };
	else if (dir == moveDir::Left)  m_direction = { round(forward.z),  0.0f, round(-forward.x) };

	const Vector3 targetPos = Vector3Add(m_cameraPosition, m_direction);
	if (!isBlocked(targetPos, map) && dir != moveDir::No)
	{
		m_isMoving = true;
		m_moveDir = dir;
		m_targetPosition = targetPos;
	}
}
//-----------------------------------------------------------------------------