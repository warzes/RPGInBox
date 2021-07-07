#pragma once

#include "Frustum.h"

class FPCamera final
{
public:
		typedef enum
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
	}CameraControls;

	FPCamera();

	void Setup(float fovY, Vector3&& position);
	void ViewResized();

	void Update();

	float GetFOVX() const;
	Vector3 GetCameraPosition() const;
	void SetCameraPosition(const Vector3&& pos);

	inline Vector2 GetViewAngles() const { return Vector2Scale(Angle, 1.0f / DEG2RAD); }



	int ControlsKeys[LAST_CONTROL];

	Vector3 MoveSpeed = { 1, 1, 1 };
	Vector2 TurnSpeed = { 90, 90 };

	float MouseSensitivity = 600;

	float MinimumViewY = -65.0f;
	float MaximumViewY = 89.0f;

	float ViewBobbleFreq = 0.0f;
	float ViewBobbleMagnatude = 0.02f;
	float ViewBobbleWaverMagnitude = 0.002f;

	typedef std::function<bool(FPCamera& camera, Vector3& newPosition, const Vector3& oldPosition)> PositionCallback;
	PositionCallback ValidateCamPosition = nullptr;

	inline const Camera& GetCamera() const { return ViewCamera; }
	inline const Frustum& GetFrustum() const { return Frustum; }

	inline void ExtractFrustum() { Frustum.Extract(); }

	// start drawing using the camera, with near/far plane support
	void BeginMode3D();

	// end drawing with the camera
	void EndMode3D();

	bool UseMouseX = true;
	bool UseMouseY = true;

	bool UseKeyboard = true;

	bool UseController = true;
	bool ControlerID = 0;

	int ControlerForwardAxis = GAMEPAD_AXIS_LEFT_Y;
	int ControllerSideAxis = GAMEPAD_AXIS_LEFT_X;
	int ControllerPitchAxis = GAMEPAD_AXIS_RIGHT_Y;
	int ControllerYawAxis = GAMEPAD_AXIS_RIGHT_X;
	int ControlerSprintButton = GAMEPAD_BUTTON_LEFT_TRIGGER_1;

	//clipping planes
	// note must use BeginMode3D and EndMode3D on the camera object for clipping planes to work
	double NearPlane = 0.01;
	double FarPlane = 1000;

	bool HideCursor = true;

private:
	float getSpeedForAxis(CameraControls axis, float speed);

	bool Focused = true;
	Vector3 CameraPosition = { 0.0f,0.0f,0.0f };

	Camera ViewCamera = { 0 };
	Vector2 FOV = { 0.0f, 0.0f };

	Vector2 PreviousMousePosition = { 0.0f, 0.0f };

	float TargetDistance = 0;               // Camera distance from position to target
	float PlayerEyesPosition = 0.5f;       // Player eyes position from ground (in meters)
	Vector2 Angle = { 0,0 };                // Camera angle in plane XZ

	float CurrentBobble = 0;

	Frustum Frustum;


};