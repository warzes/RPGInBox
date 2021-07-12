#pragma once

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

	void Setup(const float fovY, Vector3&& position, float currentRotateY = 0.0f) noexcept;
	void Update() noexcept;

	Vector3 GetCameraPosition() const noexcept
	{
		return m_cameraPosition;
	}

	void SetCameraPosition(const Vector3&& pos) noexcept;

	inline const Camera& GetCamera() const noexcept { return m_viewCamera; }
	inline const Frustum& GetFrustum() const noexcept { return m_frustum; }

	inline void ExtractFrustum() noexcept { m_frustum.Extract(); }

	int ControlsKeys[LAST_CONTROL];

	Vector3 MoveSpeed = { 1.0f, 1.0f, 1.0f };
	Vector2 TurnSpeed = { 90.0f, 90.0f };

	float MouseSensitivity = 600.0f;

	float MinimumViewY = -65.0f;
	float MaximumViewY = 89.0f;

	bool UseMouseX = true;
	bool UseMouseY = true;
	bool HideCursor = true;
	bool UseKeyboard = true;

private:
	float getSpeedForAxis(CameraControls axis, float speed) noexcept;

	Vector3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	Camera m_viewCamera = { 0 };
	const float m_playerEyesPosition = 0.75f;       // Player eyes position from ground (in meters)
	bool m_windowFocused = true;

	Vector2 m_previousMousePosition = { 0.0f, 0.0f };
	Vector2 m_angle = { 0.0f, 0.0f };                // Camera angle in plane XZ

	Frustum m_frustum;

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


	bool keyDown(int key)
	{
		return m_continuedWalk && !m_isMoving && !m_isTurning ? IsKeyDown(key) : IsKeyReleased(key);
	}

	bool isBlocked(Vector3 newPosition)
	{
		//if (_map.IsWall((int)newPosition.x, (int)newPosition.z))
		//	return true;
		//else
			return false;
	}

	void move(Vector3 direction, moveDir dir) noexcept
	{
		if (dir == moveDir::Right) direction.x = -direction.x;
		if (dir == moveDir::Back) direction.z = -direction.z;

		m_direction = direction;
		const Vector3 targetPos = Vector3Add(m_cameraPosition, direction);
		if (!isBlocked(targetPos) && dir != moveDir::No)
		{			
			m_isMoving = true;
			m_moveDir = dir;
			m_targetPosition.x = round(targetPos.x);
			m_targetPosition.y = round(targetPos.y);
			m_targetPosition.z = round(targetPos.z);
		}
	}

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


	bool m_continuedWalk = false;
	float m_walkSpeed = 3.0f;
	float m_stepSize = 1.0f;

	bool m_isMoving = false;
	bool m_isTurning = false;

	Vector3 m_targetPosition = { 0.0f, 0.0f, 0.0f };
	moveDir m_moveDir = moveDir::No;
	Vector3 m_direction = { 0 };

	rotateDirY m_rotateDir = rotateDirY::No;
	float m_endRotateY = 0.0f;
	float m_speedRotate = 180.0f;
#endif
	float m_currentRotateY = 0.0f;
};