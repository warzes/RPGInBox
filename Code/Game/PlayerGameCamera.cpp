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
	m_viewCamera.position = { m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z };
	m_viewCamera.position.y += m_playerEyesPosition;
	m_viewCamera.target = Vector3Add(m_viewCamera.position, Vector3{ 0.0f, 0.0f, 1.0f });
	m_viewCamera.up = { 0.0f, 1.0f, 0.0f };
	m_viewCamera.fovy = fovY;
	m_viewCamera.projection = CAMERA_PERSPECTIVE;

	m_windowFocused = IsWindowFocused();
	if (HideCursor && m_windowFocused && (UseMouseX || UseMouseY))
		DisableCursor();

	m_targetDistance = 1.0f;

	ViewResized();
	m_previousMousePosition = GetMousePosition();
}
//-----------------------------------------------------------------------------
void PlayerGameCamera::ViewResized() noexcept
{
	const float width = (float)GetScreenWidth();
	const float height = (float)GetScreenHeight();

	m_FOV.y = m_viewCamera.fovy;

	if (height != 0)
		m_FOV.x = m_FOV.y * (width / height);
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

	// Mouse movement detection
	Vector2 mousePositionDelta = { 0.0f, 0.0f };
	Vector2 mousePosition = GetMousePosition();
	float mouseWheelMove = GetMouseWheelMove();

	// Keys input detection
	float direction[MOVE_DOWN + 1] = { getSpeedForAxis(MOVE_FRONT,MoveSpeed.z),
	getSpeedForAxis(MOVE_BACK,MoveSpeed.z),
	getSpeedForAxis(MOVE_RIGHT,MoveSpeed.x),
	getSpeedForAxis(MOVE_LEFT,MoveSpeed.x),
	getSpeedForAxis(MOVE_UP,MoveSpeed.y),
	getSpeedForAxis(MOVE_DOWN,MoveSpeed.y) };

	if (UseController && IsGamepadAvailable(ControlerID))
	{
		bool sprint = IsKeyDown(ControlsKeys[SPRINT]) || IsGamepadButtonDown(ControlerID, ControlerSprintButton);
		float factor = MoveSpeed.z * GetFrameTime();
		if (sprint)
			factor *= 2;

		float forward = -GetGamepadAxisMovement(ControlerID, ControlerForwardAxis) * factor;
		if (forward > 0)
		{
			direction[MOVE_FRONT] = std::max(direction[MOVE_FRONT], forward);
			direction[MOVE_BACK] = 0;
		}
		else if (forward < 0)
		{
			direction[MOVE_BACK] = std::max(direction[MOVE_BACK], fabs(forward));
			direction[MOVE_FRONT] = 0;
		}

		factor = MoveSpeed.x * GetFrameTime();
		if (sprint)
			factor *= 2;

		float side = GetGamepadAxisMovement(ControlerID, ControllerSideAxis) * factor;
		if (side > 0)
		{
			direction[MOVE_RIGHT] = std::max(direction[MOVE_RIGHT], side);
			direction[MOVE_LEFT] = 0;
		}
		else if (side < 0)
		{
			direction[MOVE_LEFT] = std::max(direction[MOVE_LEFT], fabs(side));
			direction[MOVE_RIGHT] = 0;
		}
	}

	mousePositionDelta.x = mousePosition.x - m_previousMousePosition.x;
	mousePositionDelta.y = mousePosition.y - m_previousMousePosition.y;

	m_previousMousePosition = mousePosition;

	Vector3 forward = Vector3Subtract(m_viewCamera.target, m_viewCamera.position);
	forward.y = 0;
	forward = Vector3Normalize(forward);

	Vector3 right{ forward.z * -1.0f, 0, forward.x };

	Vector3 oldPosition = m_cameraPosition;

	m_cameraPosition = Vector3Add(m_cameraPosition, Vector3Scale(forward, direction[MOVE_FRONT] - direction[MOVE_BACK]));
	m_cameraPosition = Vector3Add(m_cameraPosition, Vector3Scale(right, direction[MOVE_RIGHT] - direction[MOVE_LEFT]));

	m_cameraPosition.y += direction[MOVE_UP] - direction[MOVE_DOWN];

	// let someone modify the projected position
	if (ValidateCamPosition != nullptr)
		ValidateCamPosition(*this, m_cameraPosition, oldPosition);

	// Camera orientation calculation
	float turnRotation = getSpeedForAxis(TURN_RIGHT, TurnSpeed.x) - getSpeedForAxis(TURN_LEFT, TurnSpeed.x);
	float tiltRotation = getSpeedForAxis(TURN_UP, TurnSpeed.y) - getSpeedForAxis(TURN_DOWN, TurnSpeed.y);

	if (UseController && IsGamepadAvailable(ControlerID))
	{
		float factor = GetFrameTime() * TurnSpeed.x;

		float turn = GetGamepadAxisMovement(ControlerID, ControllerYawAxis) * factor;
		if (turn > 0)
		{
			turnRotation = std::max(turnRotation, turn);
		}
		else if (turn < 0)
		{
			turnRotation = std::min(turnRotation, turn);
		}

		factor = GetFrameTime() * TurnSpeed.y;
		float tilt = -GetGamepadAxisMovement(ControlerID, ControllerPitchAxis) * factor;
		if (tilt > 0)
		{
			tiltRotation = std::max(turnRotation, tilt);
		}
		else if (tilt < 0)
		{
			tiltRotation = std::min(turnRotation, tilt);
		}
	}

	if (turnRotation != 0)
		m_angle.x -= turnRotation * DEG2RAD;
	else if (UseMouseX && m_windowFocused)
		m_angle.x += (mousePositionDelta.x / -MouseSensitivity);

	if (tiltRotation)
		m_angle.y += tiltRotation * DEG2RAD;
	else if (UseMouseY && m_windowFocused)
		m_angle.y += (mousePositionDelta.y / -MouseSensitivity);

	// Angle clamp
	if (m_angle.y < MinimumViewY * DEG2RAD)
		m_angle.y = MinimumViewY * DEG2RAD;
	else if (m_angle.y > MaximumViewY * DEG2RAD)
		m_angle.y = MaximumViewY * DEG2RAD;

	// Recalculate camera target considering translation and rotation
	Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateXYZ(Vector3{ m_angle.y, -m_angle.x, 0 }));

	m_viewCamera.position = m_cameraPosition;

	float eyeOfset = m_playerEyesPosition;

	if (ViewBobbleFreq > 0)
	{
		float swingDelta = std::fmax(std::abs(direction[MOVE_FRONT] - direction[MOVE_BACK]), std::abs(direction[MOVE_RIGHT] - direction[MOVE_LEFT]));

		// If movement detected (some key pressed), increase swinging
		m_currentBobble += swingDelta * ViewBobbleFreq;

		constexpr float ViewBobbleDampen = 8.0f;

		eyeOfset -= sinf(m_currentBobble / ViewBobbleDampen) * ViewBobbleMagnatude;

		m_viewCamera.up.x = sinf(m_currentBobble / (ViewBobbleDampen * 2)) * ViewBobbleWaverMagnitude;
		m_viewCamera.up.z = -sinf(m_currentBobble / (ViewBobbleDampen * 2)) * ViewBobbleWaverMagnitude;
	}
	else
	{
		m_currentBobble = 0;
		m_viewCamera.up.x = 0;
		m_viewCamera.up.z = 0;
	}

	m_viewCamera.position.y += eyeOfset;

	m_viewCamera.target.x = m_viewCamera.position.x + target.x;
	m_viewCamera.target.y = m_viewCamera.position.y + target.y;
	m_viewCamera.target.z = m_viewCamera.position.z + target.z;
}
//-----------------------------------------------------------------------------
float PlayerGameCamera::GetFOVX() const noexcept
{
	return m_FOV.x;
}
//-----------------------------------------------------------------------------
Vector3 PlayerGameCamera::GetCameraPosition() const noexcept
{
	return m_cameraPosition;
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
void PlayerGameCamera::BeginMode3D() noexcept
{
	float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();
	// float aspect = (float)CORE.Window.currentFbo.width/(float)CORE.Window.currentFbo.height;

	rlDrawRenderBatchActive();			// Draw Buffers (Only OpenGL 3+ and ES2)
	rlMatrixMode(RL_PROJECTION);        // Switch to projection matrix
	rlPushMatrix();                     // Save previous matrix, which contains the settings for the 2d ortho projection
	rlLoadIdentity();                   // Reset current matrix (projection)

	if (m_viewCamera.projection == CAMERA_PERSPECTIVE)
	{
		// Setup perspective projection
		double top = RL_CULL_DISTANCE_NEAR * tan(m_viewCamera.fovy * 0.5 * DEG2RAD);
		double right = top * aspect;

		rlFrustum(-right, right, -top, top, NearPlane, FarPlane);
	}
	else if (m_viewCamera.projection == CAMERA_ORTHOGRAPHIC)
	{
		// Setup orthographic projection
		double top = m_viewCamera.fovy / 2.0;
		double right = top * aspect;

		rlOrtho(-right, right, -top, top, NearPlane, FarPlane);
	}

	// NOTE: zNear and zFar values are important when computing depth buffer values

	rlMatrixMode(RL_MODELVIEW);         // Switch back to modelview matrix
	rlLoadIdentity();                   // Reset current matrix (modelview)

	// Setup Camera view
	Matrix matView = MatrixLookAt(m_viewCamera.position, m_viewCamera.target, m_viewCamera.up);
	rlMultMatrixf(MatrixToFloat(matView));      // Multiply modelview matrix by view matrix (camera)

	rlEnableDepthTest();                // Enable DEPTH_TEST for 3D
	ExtractFrustum();
}
//-----------------------------------------------------------------------------
float PlayerGameCamera::getSpeedForAxis(CameraControls axis, float speed) noexcept
{
	if (!UseKeyboard)
		return 0;

	int key = ControlsKeys[axis];
	if (key == -1)
		return 0;

	float factor = 1.0f;
	if (IsKeyDown(ControlsKeys[SPRINT]))
		factor = 2;

	if (IsKeyDown(ControlsKeys[axis]))
		return speed * GetFrameTime() * factor;

	return 0.0f;
}
//-----------------------------------------------------------------------------