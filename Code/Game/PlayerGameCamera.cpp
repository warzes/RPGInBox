#include "stdafx.h"
#include "PlayerGameCamera.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
PlayerGameCamera::PlayerGameCamera() noexcept
	: ControlsKeys{ 'W', 'S', 'D', 'A', 'E', 'Q', KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT_SHIFT }
{
}
//-----------------------------------------------------------------------------
void PlayerGameCamera::Setup(const float fovY, Vector3&& position) noexcept
{
	m_cameraPosition = std::move(position);

	m_viewCamera.position = m_cameraPosition;
	m_viewCamera.position.y += m_playerEyesPosition;
	m_viewCamera.target = Vector3Add(m_viewCamera.position, Vector3{ 0.0f, 0.0f, 1.0f });
	m_viewCamera.up = { 0.0f, 1.0f, 0.0f };
	m_viewCamera.fovy = fovY;
	m_viewCamera.projection = CAMERA_PERSPECTIVE;

#if TURN_STEP
	m_targetPosition = m_cameraPosition;
#endif
	m_windowFocused = IsWindowFocused();
	if (HideCursor && m_windowFocused && (UseMouseX || UseMouseY))
		DisableCursor();

	m_previousMousePosition = GetMousePosition();
}
//-----------------------------------------------------------------------------
void PlayerGameCamera::Update() noexcept
{
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

#if TURN_STEP
	if (!m_isTurning)
	{
		if (!m_isMoving)
		{
			Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
			forward.y = 0;
			forward = Vector3Normalize(forward);
			Vector3 right{ forward.z * -1.0f, 0, forward.x };


			if (keyDown(ControlsKeys[MOVE_FRONT])) move(forward/*transform.forward * m_stepSize*/);
			if (keyDown(ControlsKeys[MOVE_BACK])) move({ -forward.x, -forward.y, -forward.z }/*-transform.forward * m_stepSize*/);

			if (keyDown(ControlsKeys[MOVE_UP])) move(right/*-transform.right * m_stepSize*/);
			if (keyDown(ControlsKeys[MOVE_DOWN])) move({ -right.x, -right.y, -right.z }/*transform.right * m_stepSize*/);
		}

		if (keyDown(ControlsKeys[MOVE_RIGHT])) turn(-90.0f);
		if (keyDown(ControlsKeys[MOVE_LEFT])) turn(90.0f);
	}
	if (m_isMoving)
	{
		printf("TP = %f %f %f\n", m_targetPosition.x, m_targetPosition.y, m_targetPosition.z);

		m_cameraPosition.x = round(m_targetPosition.x);
		m_cameraPosition.z = round(m_targetPosition.z);

		//if (m_cameraPosition.z == m_targetPosition.z || m_cameraPosition.z > m_targetPosition.z)
		{
			m_isMoving = false;
			m_targetPosition = m_cameraPosition;
		}

		printf("CP = %f %f %f\n", m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z);
		m_viewCamera.position = m_cameraPosition;
		m_viewCamera.position.y += m_playerEyesPosition;

		Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateXYZ(Vector3{ m_angle.y, -m_angle.x, 0 }));
		m_viewCamera.up.x = 0;
		m_viewCamera.up.z = 0;
		m_viewCamera.target.x = m_viewCamera.position.x + target.x;
		m_viewCamera.target.y = m_viewCamera.position.y + target.y;
		m_viewCamera.target.z = m_viewCamera.position.z + target.z;

		//m_viewCamera.target = Vector3Add(m_viewCamera.position, Vector3{ 0.0f, 0.0f, 1.0f });
	}
	if (m_isTurning)
	{
		m_angle.x -= m_targetRotation.y * DEG2RAD;
		Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateXYZ(Vector3{ m_angle.y, -m_angle.x, 0 }));
		m_viewCamera.up.x = 0;
		m_viewCamera.up.z = 0;
		m_viewCamera.target.x = m_viewCamera.position.x + target.x;
		m_viewCamera.target.y = m_viewCamera.position.y + target.y;
		m_viewCamera.target.z = m_viewCamera.position.z + target.z;

		//transform.rotation = Quaternion.RotateTowards(transform.rotation, Quaternion.Euler(_targetRotation), Time.deltaTime * _turnSpeed);

		//if (Vector3.Distance(transform.eulerAngles, _targetRotation) < 0.05f)
		///{
			m_isTurning = false;
		//	transform.eulerAngles = _targetRotation;
		//}
	}

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
void PlayerGameCamera::SetCameraPosition(const Vector3&& pos) noexcept
{
	m_cameraPosition = std::move(pos);
	const Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
	m_viewCamera.position = m_cameraPosition;
	m_viewCamera.target = Vector3Add(m_cameraPosition, forward);
}
//-----------------------------------------------------------------------------
float PlayerGameCamera::getSpeedForAxis(CameraControls axis, float speed) noexcept
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