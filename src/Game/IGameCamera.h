#pragma once

#include "Frustum.h"

class WorldAdventure;

class IGameCamera
{
public:
	enum CameraControls
	{
		MOVE_FRONT = 0,
		MOVE_BACK,
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_UP,
		MOVE_DOWN,
		TURN_LEFT,
		TURN_RIGHT,
		TURN_UP,
		TURN_DOWN,
		SPRINT,
		LAST_CONTROL
	};
	IGameCamera() : m_controlsKeys{ 'W', 'S', 'D', 'A', 'E', 'Q', KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT_SHIFT }	{}

	virtual ~IGameCamera() = default;

	virtual void Update(const WorldAdventure& world) noexcept = 0;

	void SetCameraPosition(const Vector3& pos, float currentRotateY = 0.0f) noexcept
	{
		m_currentRotateY = currentRotateY;
		m_cameraPosition = pos;
		m_viewCamera.position = m_cameraPosition;
		m_viewCamera.position.y += m_playerEyesPosition;
		const Vector3 target = Vector3Transform({ 0.0f, 0.0f, 1.0f }, MatrixRotateY(m_currentRotateY * DEG2RAD));
		m_viewCamera.target.x = m_viewCamera.position.x + target.x;
		m_viewCamera.target.y = m_viewCamera.position.y + target.y;
		m_viewCamera.target.z = m_viewCamera.position.z + target.z;
	}
	Vector3 GetCameraPosition() const noexcept
	{
		return m_cameraPosition;
	}

	const Camera& GetCamera() const noexcept { return m_viewCamera; }
	const Frustum& GetFrustum() const noexcept { return m_frustum; }

	void ExtractFrustum() noexcept { m_frustum.Extract(); }

protected:
	const float m_playerEyesPosition = 0.75f;       // Player eyes position from ground (in meters)

	int m_controlsKeys[LAST_CONTROL] = { 0 };

	Vector3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	Camera m_viewCamera = {};
	Frustum m_frustum;
	float m_currentRotateY = 0.0f;
};