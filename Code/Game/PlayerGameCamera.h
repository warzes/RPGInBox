#pragma once

class PlayerGameCamera final
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

	PlayerGameCamera() noexcept;

	void Setup(const float fovY, Vector3&& position) noexcept;

	void ViewResized() noexcept;

	void Update() noexcept;

	float GetFOVX() const noexcept;
	Vector3 GetCameraPosition() const noexcept;
	void SetCameraPosition(const Vector3&& pos) noexcept;

	inline Vector2 GetViewAngles() const noexcept { return Vector2Scale(m_angle, 1.0f / DEG2RAD); }

	inline const Camera& GetCamera() const noexcept { return m_viewCamera; }
	inline const Frustum& GetFrustum() const noexcept { return m_frustum; }

	inline void ExtractFrustum() noexcept { m_frustum.Extract(); }

	// start drawing using the camera, with near/far plane support
	void BeginMode3D() noexcept;

	int ControlsKeys[LAST_CONTROL];

	Vector3 MoveSpeed = { 1.0f, 1.0f, 1.0f };
	Vector2 TurnSpeed = { 90.0f, 90.0f };

	float MouseSensitivity = 600.0f;

	float MinimumViewY = -65.0f;
	float MaximumViewY = 89.0f;

	float ViewBobbleFreq = 0.0f;
	float ViewBobbleMagnatude = 0.02f;
	float ViewBobbleWaverMagnitude = 0.002f;

	//clipping planes
	// note must use BeginMode3D and EndMode3D on the camera object for clipping planes to work
	double NearPlane = 0.01;
	double FarPlane = 1000;

	int ControlerForwardAxis = GAMEPAD_AXIS_LEFT_Y;
	int ControllerSideAxis = GAMEPAD_AXIS_LEFT_X;
	int ControllerPitchAxis = GAMEPAD_AXIS_RIGHT_Y;
	int ControllerYawAxis = GAMEPAD_AXIS_RIGHT_X;
	int ControlerSprintButton = GAMEPAD_BUTTON_LEFT_TRIGGER_1;

	bool UseMouseX = true;
	bool UseMouseY = true;
	bool HideCursor = true;

	bool UseKeyboard = true;

	bool UseController = true;
	bool ControlerID = 0;

	typedef std::function<bool(PlayerGameCamera& camera, Vector3& newPosition, const Vector3& oldPosition)> PositionCallback;
	PositionCallback ValidateCamPosition = nullptr;

private:
	float getSpeedForAxis(CameraControls axis, float speed) noexcept;

	Vector3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	Camera m_viewCamera = { 0 };
	const float m_playerEyesPosition = 0.5f;       // Player eyes position from ground (in meters)
	bool m_windowFocused = true;
	float m_targetDistance = 0.0f;               // Camera distance from position to target
	Vector2 m_previousMousePosition = { 0.0f, 0.0f };
	Vector2 m_FOV = { 0.0f, 0.0f };
	Vector2 m_angle = { 0.0f, 0.0f };                // Camera angle in plane XZ

	float m_currentBobble = 0.0f;

	Frustum m_frustum;
};