#include "stdafx.h"
#include "PlayerGameCamera3D.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
PlayerGameCamera3D::PlayerGameCamera3D() noexcept
	: ControlsKeys{ 'W', 'S', 'D', 'A', 'E', 'Q', KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT_SHIFT }
{
}
//-----------------------------------------------------------------------------
void PlayerGameCamera3D::Setup(const float fovY, Vector3&& position, float currentRotateY) noexcept
{
	m_currentRotateY = currentRotateY;
	m_cameraPosition = std::move(position);

	m_viewCamera.position = m_cameraPosition;
	m_viewCamera.position.y += m_playerEyesPosition;
	Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateY(m_currentRotateY * DEG2RAD));
	m_viewCamera.target.x = m_viewCamera.position.x + target.x;
	m_viewCamera.target.y = m_viewCamera.position.y + target.y;
	m_viewCamera.target.z = m_viewCamera.position.z + target.z;

	m_viewCamera.up = { 0.0f, 1.0f, 0.0f };
	m_viewCamera.fovy = fovY;
	m_viewCamera.projection = CAMERA_PERSPECTIVE;

#if TURN_STEP
	m_targetPosition = m_cameraPosition;
#endif
#if !TURN_STEP
	m_windowFocused = IsWindowFocused();
	if (HideCursor && m_windowFocused && (UseMouseX || UseMouseY))
		DisableCursor();
#endif

	m_previousMousePosition = GetMousePosition();
}
//-----------------------------------------------------------------------------
void PlayerGameCamera3D::Update() noexcept
{
#if !TURN_STEP
	if (HideCursor && IsWindowFocused() != m_windowFocused && (UseMouseX || UseMouseY))
	{
		m_windowFocused = IsWindowFocused();
		if (m_windowFocused)
		{
			DisableCursor();
			m_previousMousePosition = GetMousePosition(); // so there is no jump on focus
		}
		else
		{
			EnableCursor();
		}
	}
#endif

#if TURN_STEP
	if (!m_isTurning) 
	{
		if (!m_isMoving) // во время вращения камеры нельзя двигаться, поэтому условие вложенное
		{
			if (keyDown(ControlsKeys[MOVE_FRONT])) move(moveDir::Forward);
			if (keyDown(ControlsKeys[MOVE_BACK])) move(moveDir::Back);

			if (keyDown(ControlsKeys[MOVE_UP])) move(moveDir::Left);
			if (keyDown(ControlsKeys[MOVE_DOWN])) move(moveDir::Right);
		}

		if (keyDown(ControlsKeys[MOVE_LEFT])) turn(rotateDirY::Left);
		if (keyDown(ControlsKeys[MOVE_RIGHT])) turn(rotateDirY::Right);
	}
	if (m_isMoving)
	{
		m_cameraPosition.x += m_walkSpeed * m_direction.x * GetFrameTime();
		m_cameraPosition.z += m_walkSpeed * m_direction.z * GetFrameTime();

		if (m_direction.x > 0 && m_cameraPosition.x >= m_targetPosition.x)
		{
			m_isMoving = false;
			m_cameraPosition.x = m_targetPosition.x;
		}
		if (m_direction.x < 0 && m_cameraPosition.x <= m_targetPosition.x)
		{
			m_isMoving = false;
			m_cameraPosition.x = m_targetPosition.x;
		}

		if (m_direction.z > 0 && m_cameraPosition.z >= m_targetPosition.z)
		{
			m_isMoving = false;
			m_cameraPosition.z = m_targetPosition.z;
		}
		if (m_direction.z < 0 && m_cameraPosition.z <= m_targetPosition.z)
		{
			m_isMoving = false;
			m_cameraPosition.z = m_targetPosition.z;
		}

		//m_cameraPosition.x = m_targetPosition.x;
		//m_cameraPosition.z = m_targetPosition.z;
		
				
		//printf("TP = %f %f\n", m_direction.x, m_direction.z);
		printf("CP = %f %f\n", m_cameraPosition.x, m_cameraPosition.z);
		m_viewCamera.position = m_cameraPosition;
		m_viewCamera.position.y = m_playerEyesPosition;
	}
	if (m_isTurning)
	{
		m_currentRotateY += m_speedRotate 
			* (m_rotateDir == rotateDirY::Left ? -1.0f : 1.0f) 
			* GetFrameTime();

		if ((m_rotateDir == rotateDirY::Right && m_currentRotateY >= m_endRotateY) ||
			(m_rotateDir == rotateDirY::Left  && m_currentRotateY <= m_endRotateY))
		{
			m_isTurning = false;
			m_rotateDir = rotateDirY::No;
			m_currentRotateY = m_endRotateY;
		}
	}

	const Vector3 target = Vector3Transform({ 0.0f, 0.0f, 1.0f }, MatrixRotateY(m_currentRotateY*DEG2RAD));
	m_viewCamera.target.x = m_viewCamera.position.x + target.x;
	m_viewCamera.target.y = m_viewCamera.position.y + target.y;
	m_viewCamera.target.z = m_viewCamera.position.z + target.z;
	
#else
	// Keys input detection
	float direction[MOVE_DOWN + 1] = 
	{ 
		getSpeedForAxis(MOVE_FRONT,MoveSpeed.z),
		getSpeedForAxis(MOVE_BACK,MoveSpeed.z),
		getSpeedForAxis(MOVE_RIGHT,MoveSpeed.x),
		getSpeedForAxis(MOVE_LEFT,MoveSpeed.x),
		getSpeedForAxis(MOVE_UP,MoveSpeed.y),
		getSpeedForAxis(MOVE_DOWN,MoveSpeed.y) 
	};

	Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
	forward.y = 0;
	forward = Vector3Normalize(forward);
	Vector3 right{ forward.z * -1.0f, 0, forward.x };

	m_cameraPosition = Vector3Add(m_cameraPosition, Vector3Scale(forward, direction[MOVE_FRONT] - direction[MOVE_BACK]));
	m_cameraPosition = Vector3Add(m_cameraPosition, Vector3Scale(right, direction[MOVE_RIGHT] - direction[MOVE_LEFT]));
	m_cameraPosition.y += direction[MOVE_UP] - direction[MOVE_DOWN];

	m_viewCamera.position = m_cameraPosition;
	m_viewCamera.position.y += m_playerEyesPosition;






	// Mouse movement detection
	Vector2 mousePositionDelta = { 0.0f, 0.0f };
	Vector2 mousePosition = GetMousePosition();
	float mouseWheelMove = GetMouseWheelMove();
		
	mousePositionDelta.x = mousePosition.x - m_previousMousePosition.x;
	mousePositionDelta.y = mousePosition.y - m_previousMousePosition.y;

	m_previousMousePosition = mousePosition;
	

	// Camera orientation calculation
	float turnRotation = getSpeedForAxis(TURN_RIGHT, TurnSpeed.x) - getSpeedForAxis(TURN_LEFT, TurnSpeed.x);
	float tiltRotation = getSpeedForAxis(TURN_UP, TurnSpeed.y) - getSpeedForAxis(TURN_DOWN, TurnSpeed.y);
	
	if (turnRotation != 0)
		m_angle.x -= turnRotation * DEG2RAD;
	else if (UseMouseX && m_windowFocused)
		m_angle.x += (mousePositionDelta.x / -MouseSensitivity);

	//if (tiltRotation)
	//	m_angle.y += tiltRotation * DEG2RAD;
	//else if (UseMouseY && m_windowFocused)
	//	m_angle.y += (mousePositionDelta.y / -MouseSensitivity);

	// Angle clamp
	if (m_angle.y < MinimumViewY * DEG2RAD)
		m_angle.y = MinimumViewY * DEG2RAD;
	else if (m_angle.y > MaximumViewY * DEG2RAD)
		m_angle.y = MaximumViewY * DEG2RAD;

	// Recalculate camera target considering translation and rotation
	Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateXYZ(Vector3{ m_angle.y, -m_angle.x, 0 }));
				
	m_viewCamera.up.x = 0;
	m_viewCamera.up.z = 0;
	m_viewCamera.target.x = m_viewCamera.position.x + target.x;
	m_viewCamera.target.y = m_viewCamera.position.y + target.y;
	m_viewCamera.target.z = m_viewCamera.position.z + target.z;

#endif
}
//-----------------------------------------------------------------------------
void PlayerGameCamera3D::SetCameraPosition(const Vector3&& pos) noexcept
{
	m_cameraPosition = std::move(pos);
	const Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
	m_viewCamera.position = m_cameraPosition;
	m_viewCamera.target = Vector3Add(m_cameraPosition, forward);
}
//-----------------------------------------------------------------------------
float PlayerGameCamera3D::getSpeedForAxis(CameraControls axis, float speed) noexcept
{
	if (!UseKeyboard)
		return 0;
	int key = ControlsKeys[axis];
	if (key == -1) return 0;

	float factor = 1.0f;
	if (IsKeyDown(ControlsKeys[SPRINT]))
		factor = 2;

	if (IsKeyDown(ControlsKeys[axis]))
		return speed * GetFrameTime() * factor;

	return 0.0f;
}
//-----------------------------------------------------------------------------