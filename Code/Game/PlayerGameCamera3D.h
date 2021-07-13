#pragma once

class World;
class Map;

class PlayerGameCamera3D final
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

	PlayerGameCamera3D() noexcept;

	void Setup(const float fovY, const Vector3& position, const float currentRotateY = 0.0f) noexcept;
	void Update(World& world) noexcept;

	Vector3 GetCameraPosition() const noexcept
	{
		return m_cameraPosition;
	}

	void SetCameraPosition(const Vector3& pos, float currentRotateY = 0.0f) noexcept;

	const Camera& GetCamera() const noexcept { return m_viewCamera; }
	const Frustum& GetFrustum() const noexcept { return m_frustum; }

	void ExtractFrustum() noexcept { m_frustum.Extract(); }

	int ControlsKeys[LAST_CONTROL];

#if !TURN_STEP
	Vector3 MoveSpeed = { 1.0f, 1.0f, 1.0f };
	Vector2 TurnSpeed = { 90.0f, 90.0f };

	float MouseSensitivity = 600.0f;

	float MinimumViewY = -65.0f;
	float MaximumViewY = 89.0f;

	bool UseMouseX = true;
	bool UseMouseY = true;
	bool HideCursor = true;
	bool UseKeyboard = true;
#endif

private:
#if !TURN_STEP
	float getSpeedForAxis(CameraControls axis, float speed) noexcept;
#endif
	Vector3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	Camera m_viewCamera = { 0 };
	Frustum m_frustum;
	const float m_playerEyesPosition = 0.75f;       // Player eyes position from ground (in meters)
#if !TURN_STEP
	bool m_windowFocused = true;
	Vector2 m_previousMousePosition = { 0.0f, 0.0f };
	Vector2 m_angle = { 0.0f, 0.0f };                // Camera angle in plane XZ
#endif

#if TURN_STEP
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
#endif
	float m_currentRotateY = 0.0f;
};